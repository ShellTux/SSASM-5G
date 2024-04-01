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

#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>

void authorizationRequestsManager(const int sharedMemoryID)
{
	logMessage(LOG_AUTHORIZATION_REQUESTS_MANAGER_PROCESS_CREATED);

	sleep(1);

	char *sharedMemory;
	if ((sharedMemory = shmat(sharedMemoryID, NULL, 0)) == (char *) -1) {
		perror("IPC error: shmat");
		exit(EXIT_FAILURE);
	}

	sleep(3);

	if (shmdt(sharedMemory) == -1) {
		perror("IPC error: shmdt");
		exit(EXIT_FAILURE);
	}

	sleep(1);
}
