/***************************************************************************
 * Project          ____ ____    _    ____  __  __      ____   ____
 *                 / ___/ ___|  / \  / ___||  \/  |    | ___| / ___|
 *                 \___ \___ \ / _ \ \___ \| |\/| |____|___ \| |  _
 *                  ___) |__) / ___ \ ___) | |  | |_____|__) | |_| |
 *                 |____/____/_/   \_\____/|_|  |_|    |____/ \____|
 *
 *
 * Author: David Carvalheiro, 2022220112
 * Author: Luís Góis,         2018280716
 *
 * This software is licensed as described in the file LICENSE, which
 * you should have received as part of this distribution.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the LICENSE file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/

#include "MobileUser.h"

#include "AuthorizationRequest.h"
#include "IPCS/MessageQueue.h"
#include "IPCS/Pipes.h"
#include "utils/error.h"

#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/param.h>
#include <unistd.h>

int userPipeFD;
int messageQueueID;
/**
 * Função principal responsável pela inicialização do MobileUser e pelo envio de pedidos de serviço
 * @param argc o número de argumentos de linha de comando
 * @param argv um array de strings contendo os elementos para um pedido de serviço 
 */

int main(int argc, char *argv[])
{
	for (int i = 0; i < argc; ++i) {
		const char *const arg = argv[i];
		if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
			usage(argv[0]);
		}
	}

	if (argc != 7) {
		usage(argv[0]);
	}

	if ((userPipeFD = open(USER_PIPE, O_WRONLY)) < 0) {
		HANDLE_ERROR("open: " USER_PIPE);
	}

	messageQueueID = openMessageQueue();

	signal(SIGINT, sigintHandler);

	MobileUser mobileUser = createMobileUserFromArgs(argv + 1, argc - 1);

	sendDataServiceRequests(mobileUser);

	return EXIT_SUCCESS;
}
/**
 * Imprime as instruções de uso do mobileUser
 * @param programName o nome do programa
 */

void usage(const char *const programName)
{
	fprintf(stderr,
	        "Usage: %s [options] <plafond_inicial> <num_pedidos> "
	        "<intervalo_video> <intervalo_music> <intervalo_social> "
	        "<dados_reservar>\n",
	        programName);
	fprintf(stderr, "Options:\n");
	fprintf(stderr,
	        "  -h, --help                   Print this usage message\n");
	exit(EXIT_FAILURE);
}
/**
 * Manipulador de sinal para SIGINT (Ctrl+C).
 * @param signal o sinal recebido
 */

void sigintHandler(const int signal)
{
	(void) signal;
	printf("Received SIGINT. Exiting...\n");
	cleanup();
	exit(EXIT_SUCCESS);
}
/**
 * Envia um pedido de autorização para o userPipe 
 * @param request a solicitação de autorização a ser enviada
 */

void sendMessage(const AuthorizationRequest request)
{
	char buffer[1025] = {0};
	sprintf(buffer,
	        AUTHORIZATION_REQUEST_MESSAGE_FORMAT_SEND,
	        request.mobileUserID,
	        serviceString(request.service),
	        request.reservingData);
	write(userPipeFD, buffer, sizeof(buffer));
}
/**
 * Cria um MobileUser a partir dos argumentos de linha de comando
 * @param arguments um array de strings contendo os argumentos
 * @param argumentsLength o número de argumentos
 * @return o MobileUser criado
 */

MobileUser createMobileUserFromArgs(char **arguments, const int argumentsLength)
{
	MobileUser mobileUser;

	const int N = MIN(argumentsLength, MOBILE_USER_OPTIONS_NUM);
	for (int i = 0; i < N; ++i) {
		const char *const argument = arguments[i];

		const int input = atoi(argument);
		if (input < 0) {
			fprintf(stderr,
			        "Invalid argument: %s. Must be a positive "
			        "integer.\n",
			        argument);
		}

		mobileUser.arrayOptions[i] = atoi(argument);
	}

	mobileUser.options.processID = getpid();

	static const MobileUser invalidMobileUser = {
	    .arrayOptions = {0},
	};

	return isMobileUserValid(mobileUser) ? mobileUser : invalidMobileUser;
}
/**
 * Verifica se um MobileUser é válido.
 * @param mobileUser o MobileUser a ser verificado
 * @return true se o MobileUser for válido, false caso contrário
 */

bool isMobileUserValid(const MobileUser mobileUser)
{
	(void) mobileUser;
	return true;
}
/**
 * Imprime as informações de um MobileUser em um ficheiro
 * @param file o ficheiro onde as informações serão impressas
 * @param mobileUser o MobileUser cujas informações serão impressas
 */

void printMobileUser(FILE *file, const MobileUser mobileUser)
{
	fprintf(file,
	        MOBILE_USER_PRINT_FORMAT "\n",
	        mobileUser.options.plafondInicial,
	        mobileUser.options.numPedidos,
	        mobileUser.options.intervalVideo,
	        mobileUser.options.intervalMusic,
	        mobileUser.options.intervalSocial,
	        mobileUser.options.reservedData,
	        mobileUser.options.userID);
}
/**
 * Escuta por alertas de mensagem na fila de mensagens e imprime os alertas recebidos.
 */

void listenToMessageAlerts(void)
{
	Message message;

	bool running = true;
	while (running) {
		if (msgrcv(messageQueueID,
		           &message,
		           MESSAGE_SIZE,
		           ALERT_MESSAGE_TYPE,
		           0)
		    < 0) {
			HANDLE_ERROR("msgrcv: ");

			printf("%s\n", dataCapAlertToString(message.alert));

			switch (message.alert) {
			case ALERT_100:
			case EXIT:
				running = 0;
				break;

			case OK:
			case ALERT_90:
			case ALERT_80:
				break;
			}
		}
	}
}
/**
 * Envia os pedidos de autorização com base nas opções do MobileUser
 * @param mobileUser o MobileUser que faz o pedido de autorização 
 */

void sendDataServiceRequests(const MobileUser mobileUser)
{
	size_t authorizationRequests = 0;
	while (true) {
		if (authorizationRequests > mobileUser.options.numPedidos) {
			break;
		}

#define SERVICE(ENUM, STRING)                                 \
	sendMessage((AuthorizationRequest){                   \
	    .mobileUserID  = mobileUser.options.userID,       \
	    .reservingData = mobileUser.options.reservedData, \
	    .service       = ENUM,                            \
	});                                                   \
	authorizationRequests++;
		SERVICES
#undef SERVICE
	}
}

void cleanup(void) {}
