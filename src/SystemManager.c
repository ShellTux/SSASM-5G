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

#include "SystemManager.h"

#include "AuthorizationRequestsManager.h"
#include "SystemManager/config.h"
#include "log.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	for (int i = 0; i < argc; ++i) {
		const char *const arg = argv[i];
		if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
			usage(argv[0]);
		}
	}

	if (argc != 2) {
		usage(argv[0]);
	}

	const char *const configFilepath = argv[1];

	if (!isValidSystemManagerConfigFile(configFilepath)) {
		exit(EXIT_FAILURE);
	}

	logMessage(LOG_SIMULATOR_START);

	logMessage(LOG_SYSTEM_MANAGER_PROCESS_CREATED);

	int shmid;
	if ((shmid = shmget(SHARED_MEMORY_KEY,
	                    SHARED_MEMORY_SIZE,
	                    SHARED_MEMORY_PERMISSIONS | IPC_CREAT))
	    < 0) {
		perror("IPC error: shmget");
		exit(EXIT_FAILURE);
	}

	pid_t pid = fork();
	if (pid < 0) {
		perror("Fork failed");
		exit(EXIT_FAILURE);
	} else if (pid == 0) {
		authorizationRequestsManager(shmid);
		exit(EXIT_SUCCESS);
	}

	char *sharedMemory;
	if ((sharedMemory = shmat(shmid, NULL, 0)) == (char *) -1) {
		perror("IPC error: shmat");
		exit(EXIT_FAILURE);
	}

	int status;
	while ((wait(&status)) > 0) {}

	if (shmdt(sharedMemory) == -1) {
		perror("IPC error: shmdt");
		exit(EXIT_FAILURE);
	}

	if (shmctl(shmid, IPC_RMID, NULL) == -1) {
		perror("IPC error: shmctl");
		exit(EXIT_FAILURE);
	}

	logMessage(LOG_SIMULATOR_END);

	return EXIT_SUCCESS;
}

void usage(const char *const programName)
{
	printf("Usage: %s [options] <config file>\n", programName);
	printf("Options:\n");
	printf("  -h, --help                   Print this usage message\n");

	exit(EXIT_FAILURE);
}
