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

#include <bits/pthreadtypes.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void authorizationRequestsManager(void)
{
	logMessage(LOG_AUTHORIZATION_REQUESTS_MANAGER_PROCESS_CREATED);

	createNamedPipe(USER_PIPE, USER_PIPE_PERMISSIONS);
	createNamedPipe(BACK_PIPE, BACK_PIPE_PERMISSIONS);

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

	unlink(USER_PIPE);
	unlink(BACK_PIPE);

	int status;
	while ((wait(&status)) > 0) {}
}

void *receiverThread(void *argument)
{
	(void) argument;

	logMessage(LOG_THREAD_CREATED(RECEIVER));

#define RECEIVER_THREAD_WIP 1

	int userPipeFD;
#if RECEIVER_THREAD_WIP
	if ((userPipeFD = open(USER_PIPE, O_RDONLY | O_NONBLOCK)) < 0) {
#else
	if ((userPipeFD = open(USER_PIPE, O_RDONLY)) < 0) {
#endif
		HANDLE_ERROR("open: ");
	}

	int backPipeFD;
#if RECEIVER_THREAD_WIP
	if ((backPipeFD = open(BACK_PIPE, O_RDONLY | O_NONBLOCK)) < 0) {
#else
	if ((backPipeFD = open(BACK_PIPE, O_RDONLY)) < 0) {
#endif
		HANDLE_ERROR("open: ");
	}

#undef RECEIVER_THREAD_WIP

	// TODO: Parse the message from the pipe and send it to the queues

	close(userPipeFD);
	close(backPipeFD);

	logMessage(LOG_THREAD_EXIT(RECEIVER));

	pthread_exit(NULL);
}

void *senderThread(void *argument)
{
	(void) argument;
	logMessage(LOG_THREAD_CREATED(SENDER));
	pthread_exit(NULL);
}

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
