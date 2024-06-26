#ifndef AUTHORIZATION_REQUESTS_MANAGER_H
#define AUTHORIZATION_REQUESTS_MANAGER_H

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

#include "AuthorizationEngine.h"
#include "AuthorizationRequestsManager/Queue.h"

#include <stddef.h>

#define LOG_AUTHORIZATION_REQUESTS_MANAGER_PROCESS_CREATED \
	"PROCESS AUTHORIZATION_REQUEST_MANAGER CREATED"
#define LOG_AUTHORIZATION_REQUESTS_MANAGER_PROCESS_EXIT \
	"PROCESS AUTHORIZATION_REQUEST_MANAGER EXITING..."
#define AUTHORIZATION_REQUESTS_MANAGER_CLEAN_RESOURCES_START \
	"CLEANING AUTHORIZATION REQUESTS MANAGER RESOURCES STARTING..."
#define AUTHORIZATION_REQUESTS_MANAGER_CLEAN_RESOURCES_END \
	"CLEANING AUTHORIZATION REQUESTS MANAGER RESOURCES ENDING..."

#define LOG_THREAD_CREATED(WHO) "THREAD " #WHO " CREATED"
#define LOG_THREAD_EXIT(WHO)    "THREAD " #WHO " EXITING..."

typedef struct {
	AuthorizationEngine *engines;
	size_t size;
} AuthorizationEngines;

void authorizationRequestsManager(const size_t authServersMax);
AuthorizationEngines createAuthorizationEngines(const size_t maxAuthServers);
void *receiverThread(void *argument);
void *senderThread(void *argument);

#endif // !AUTHORIZATION_REQUESTS_MANAGER_H
