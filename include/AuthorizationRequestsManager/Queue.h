#ifndef SSASM_5G_AUTHORIZATION_REQUESTS_MANAGER_QUEUE_H
#define SSASM_5G_AUTHORIZATION_REQUESTS_MANAGER_QUEUE_H

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

#include "AuthorizationRequest.h"

#include <stddef.h>

typedef struct {
	int writePos, readPos;
	AuthorizationRequest *data;
	size_t size;
} Queue;

Queue createQueue(const size_t size);
void destroyQueue(Queue *queue);

#endif // !SSASM_5G_AUTHORIZATION_REQUESTS_MANAGER_QUEUE_H
