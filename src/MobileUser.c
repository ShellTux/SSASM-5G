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

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


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

	signal(SIGINT, sigintHandler);

	const int plafondInicial  = atoi(argv[1]);
	const int numPedidos      = atoi(argv[2]);
	const int intervaloVideo  = atoi(argv[3]);
	const int intervaloMusic  = atoi(argv[4]);
	const int intervaloSocial = atoi(argv[5]);
	const int dadosReservar   = atoi(argv[6]);
	const pid_t userID        = getpid();

	int authorizationRequests = 0;
	while (true) {
		if (authorizationRequests > numPedidos) break;

		sendMessage(userID, VIDEO, dadosReservar);
		authorizationRequests++;

		sendMessage(userID, MUSIC, dadosReservar);
		authorizationRequests++;

		sendMessage(userID, SOCIAL, dadosReservar);
		authorizationRequests++;
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
	exit(EXIT_FAILURE);
}

void sigintHandler(const int signal)
{
	(void) signal;
	printf("Received SIGINT. Exiting...\n");
	exit(EXIT_SUCCESS);
}

void sendMessage(const int userID,
                 const Service service,
                 const int dataReservation)
{
	printf(AUTHORIZATION_MESSAGE_FORMAT "\n",
	       userID,
	       serviceString(service),
	       dataReservation);
}

const char *serviceString(const Service service)
{
	switch (service) {
#define WRAPPER(ENUM)         \
	case ENUM: {          \
		return #ENUM; \
	};
		SERVICES
#undef WRAPPER
	}

	return NULL;
}
