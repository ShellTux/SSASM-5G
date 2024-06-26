#ifndef SSASM_5G_IPCS_SHARED_MEMORY_H
#define SSASM_5G_IPCS_SHARED_MEMORY_H

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

#include <stdbool.h>
#include <stddef.h>

#define SHARED_MEMORY_KEY         1234
#define SHARED_MEMORY_PERMISSIONS 0644

typedef struct {
	size_t id;
	size_t plafondInitial;
	size_t plafondUsed;
} MobileUserRecord;

int createSharedMemory(const size_t size, const bool zeroFill);
void *attachSharedMemory(const int id);
void detachSharedMemory(const void *sharedMemoryPointer);
void deleteSharedMemory(const int id);

#endif // !SSASM_5G_IPCS_SHARED_MEMORY_H
