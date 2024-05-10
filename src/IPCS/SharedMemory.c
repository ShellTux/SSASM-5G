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

#include "IPCS/SharedMemory.h"

#include "MobileUser.h"
#include "utils/error.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int createSharedMemory(const size_t mobileUsers)
{
	int shmid;
	if ((shmid = shmget(SHARED_MEMORY_KEY,
	                    sizeof(MobileUser) * mobileUsers,
	                    SHARED_MEMORY_PERMISSIONS | IPC_CREAT))
	    < 0) {
		HANDLE_ERROR("shmget: ");
	}

	return shmid;
}

void deleteSharedMemory(const int id)
{
	if (shmctl(id, IPC_RMID, NULL) < 0) {
		HANDLE_ERROR("shmctl: ");
	}
}
