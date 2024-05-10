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
#include "IPCS/Pipes.h"
#include "utils/error.h"

#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/param.h>
#include <unistd.h>

int userPipeFD;

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
		HANDLE_ERROR("open: ");
	}

	signal(SIGINT, sigintHandler);

	MobileUser mobileUser = createMobileUserFromArgs(argv + 1, argc - 1);

	size_t authorizationRequests = 0;
	while (true) {
		if (authorizationRequests > mobileUser.options.numPedidos) {
			break;
		}

#define SERVICE(ENUM)                                         \
	sendMessage((AuthorizationRequest){                   \
	    .mobileUserID  = mobileUser.options.userID,       \
	    .reservingData = mobileUser.options.reservedData, \
	    .service       = ENUM,                            \
	});                                                   \
	authorizationRequests++;
		SERVICES
#undef SERVICE
	}

	return EXIT_SUCCESS;
}

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

void sigintHandler(const int signal)
{
	(void) signal;
	printf("Received SIGINT. Exiting...\n");
	exit(EXIT_SUCCESS);
}

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

MobileUser createMobileUserFromArgs(char **arguments, const int argumentsLength)
{
	MobileUser mobileUser;

	const int N = MIN(argumentsLength, MOBILE_USER_OPTIONS_NUM);
	for (int i = 0; i < N; ++i) {
		const char *const argument = arguments[i];

		mobileUser.arrayOptions[i] = atoi(argument);
	}

	mobileUser.options.processID = getpid();

	static const MobileUser invalidMobileUser = {
	    .arrayOptions = {0},
	};

	return isMobileUserValid(mobileUser) ? mobileUser : invalidMobileUser;
}

bool isMobileUserValid(const MobileUser mobileUser)
{
	(void) mobileUser;
	return true;
}

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
