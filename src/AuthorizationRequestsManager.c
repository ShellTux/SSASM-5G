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

#include "AuthorizationRequestsManager.h"

#include "AuthorizationEngine.h"
#include "AuthorizationRequest.h"
#include "IPCS/Pipes.h"
#include "log.h"
#include "utils/error.h"
#include "utils/fork.h"
#include "utils/math.h"

#include <bits/pthreadtypes.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
/**
 * Função principal responsável por gerenciar os pedidos de autorização, criar as threads receiver e sender 
 * ...
 */

pthread_t threadReceiver;
pthread_t threadSender;

int userPipeFD;
int backPipeFD;

size_t authorizationEnginesMax;

static void listenForSignals(const int ansiSignal);
static void cleanResources(void);

void authorizationRequestsManager(const size_t authServersMax)
{
	logMessage(LOG_AUTHORIZATION_REQUESTS_MANAGER_PROCESS_CREATED);

	authorizationEnginesMax = authServersMax;

	createNamedPipe(USER_PIPE, USER_PIPE_PERMISSIONS);
	createNamedPipe(BACK_PIPE, BACK_PIPE_PERMISSIONS);

	pthread_create(&threadReceiver, NULL, receiverThread, NULL);
	pthread_create(&threadSender, NULL, senderThread, NULL);

	signal(SIGINT, listenForSignals);

	pthread_join(threadReceiver, NULL);
	pthread_join(threadSender, NULL);

	unlink(USER_PIPE);
	unlink(BACK_PIPE);

	int status;
	while ((wait(&status)) > 0) {}

	logMessage(LOG_AUTHORIZATION_REQUESTS_MANAGER_PROCESS_EXIT);
}
/**
 * Thread responsável por receber os pedidos de autorização dos usuários
 * @param argument não utilizado
 */

void *receiverThread(void *argument)
{
	(void) argument;

	logMessage(LOG_THREAD_CREATED(RECEIVER));

	if ((userPipeFD = open(USER_PIPE, O_RDWR)) < 0) {
		HANDLE_ERROR("open: " USER_PIPE);
	}

	if ((backPipeFD = open(BACK_PIPE, O_RDWR)) < 0) {
		HANDLE_ERROR("open: " BACK_PIPE);
	}

	// TODO: Parse the message from the pipe and send it to the queues
	fd_set readPipes;

	char buffer[PIPE_BUFFER_SIZE + 1]
	    = {[0] = '\0', [PIPE_BUFFER_SIZE] = '\0'};
	while (true) {
		FD_ZERO(&readPipes);
		FD_SET(userPipeFD, &readPipes);
		FD_SET(backPipeFD, &readPipes);

		// BUG: select: non-blocking
		if (select(MAX(userPipeFD, backPipeFD) + 1,
		           &readPipes,
		           NULL,
		           NULL,
		           NULL)
		    <= 0) {
			HANDLE_ERROR("select: ");
		}

		if (FD_ISSET(userPipeFD, &readPipes)) {
			// TODO: Parse the message from the pipe and send it to the queues
			buffer[read(userPipeFD, buffer, PIPE_BUFFER_SIZE)]
			    = '\0';
			printDebug(stdout,
			           DEBUG_INFO,
			           USER_PIPE ": %s ",
			           buffer);

			const AuthorizationRequest request
			    = parseAuthorizationRequest(buffer);
#if DEBUG
			printAuthorizationRequest(stdout, request);
#endif

			// TODO: Send to the queues
		} else if (FD_ISSET(backPipeFD, &readPipes)) {
			// TODO: Parse the message from the pipe and send it to the queues
			buffer[read(backPipeFD, buffer, PIPE_BUFFER_SIZE)]
			    = '\0';
			printDebug(stdout,
			           DEBUG_INFO,
			           BACK_PIPE ": %s\n",
			           buffer);

			// TODO: Send to the queues
		}
	}

	close(userPipeFD);
	close(backPipeFD);

	logMessage(LOG_THREAD_EXIT(RECEIVER));

	pthread_exit(NULL);
}
/**
 * Thread responsável por enviar os pedidos de autorização para os motores disponíveis
 * @param argument não utilizado
 */

void *senderThread(void *argument)
{
	(void) argument;
	logMessage(LOG_THREAD_CREATED(SENDER));

	// TODO: Create Authorization Engines
	// TODO: Read Authorization Request from queues and deliver in an
	// available AuthorizationEngine

	// TODO: Extract Authorization Request from queues and send it to the
	// AuthorizationEngine

	logMessage(LOG_THREAD_EXIT(SENDER));
	pthread_exit(NULL);
}

/**
 * Cria as engines de autorização para lidar com os pedidos de autorização
 * @param maxAuthServers o número máximo de engines de autorização
 * @return as engines de autorização criadas
 */

AuthorizationEngines createAuthorizationEngines(const size_t maxAuthServers)
{
	static const AuthorizationEngines invalidEngines = {0};

	AuthorizationEngines engines = invalidEngines;

	if (maxAuthServers <= 0) {
		return engines;
	}

	engines = (AuthorizationEngines){
	    .size    = maxAuthServers,
	    .engines = calloc(maxAuthServers, sizeof(AuthorizationEngine)),
	};

	for (size_t i = 0; i < engines.size; ++i) {
		FORK_FUNCTION(authorizationEngine);
	}

	return engines.engines == NULL ? invalidEngines : engines;
}

static void listenForSignals(const int ansiSignal)
{
	signal(ansiSignal, SIG_IGN);

	switch (ansiSignal) {
	case SIGINT: {
		cleanResources();
		logMessage(LOG_AUTHORIZATION_REQUESTS_MANAGER_PROCESS_EXIT);
		exit(EXIT_SUCCESS);
	} break;
	}

	signal(ansiSignal, listenForSignals);
}

static void cleanResources(void)
{
	printDebug(stdout,
	           DEBUG_INFO,
	           AUTHORIZATION_REQUESTS_MANAGER_CLEAN_RESOURCES_START);

	pthread_cancel(threadReceiver);
	pthread_cancel(threadSender);

	// TODO: Destroy queue mutex and condition

	close(userPipeFD);
	close(backPipeFD);

	unlink(USER_PIPE);
	unlink(BACK_PIPE);

	// TODO: Wait for all AuthorizationEngines to finish
	// TODO: Empty the internal queues
	// TODO: Clean the AuthorizationEngines
	// TODO: Clean the internal queues

	printDebug(stdout,
	           DEBUG_INFO,
	           AUTHORIZATION_REQUESTS_MANAGER_CLEAN_RESOURCES_END);
}
