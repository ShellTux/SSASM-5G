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

#include "AuthorizationRequest.h"
#include "log.h"
#include "utils/error.h"

#include <bits/pthreadtypes.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void authorizationRequestsManager(const int sharedMemoryID)
{
	logMessage(LOG_AUTHORIZATION_REQUESTS_MANAGER_PROCESS_CREATED);

	if (mkfifo(USER_PIPE, O_CREAT | O_EXCL | USER_PIPE_PERMISSIONS) < 0
	    && (errno != EEXIST)) {
		perror("mkfifo: ");
	}

	if (mkfifo(BACK_PIPE, O_CREAT | O_EXCL | BACK_PIPE_PERMISSIONS) < 0
	    && (errno != EEXIST)) {
		perror("mkfifo: ");
	}

	pthread_t threads[2];

	// NOLINTNEXTLINE
#define receiver 0
	// NOLINTNEXTLINE
#define sender   1
	pthread_create(&threads[receiver], NULL, receiverThread, NULL);
	pthread_create(&threads[sender], NULL, senderThread, NULL);

	pthread_join(threads[receiver], NULL);
	pthread_join(threads[sender], NULL);
#undef receiver
#undef sender

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

	unlink(USER_PIPE);
	unlink(BACK_PIPE);
}

void *receiverThread(void *argument)
{
	(void) argument;
	logMessage(LOG_THREAD_CREATED(RECEIVER));

	int userPipeFD;
	if ((userPipeFD = open(USER_PIPE, O_RDONLY)) < 0) {
		HANDLE_ERROR("open: ");
	}

	char buffer[4097] = {0};
	read(userPipeFD, buffer, 4096);
	AuthorizationRequest request
	    = createAuthorizationRequestFromString(buffer);
	printAuthorizationRequest(stdout, request);

	pthread_exit(NULL);
}

void *senderThread(void *argument)
{
	(void) argument;
	logMessage(LOG_THREAD_CREATED(SENDER));
	pthread_exit(NULL);
}
