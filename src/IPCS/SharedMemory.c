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

#include "log.h"
#include "utils/error.h"

#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

const char *const SHARED_MEMORY_MUTEX = "shared-memory-mutex";
sem_t *sharedMemoryMutex              = NULL;

/**
 * Inicializa o mutex para a memória compartilhada.
 */
static void initSharedMemoryMutex(void)
{
	if (sharedMemoryMutex != NULL) {
		printDebug(stdout,
		           DEBUG_WARNING,
		           "Shared memory mutex already created\n");
		return;
	}

	if (sem_unlink(SHARED_MEMORY_MUTEX) < 0) {
		if (errno != ENOENT) {
			HANDLE_ERROR("sem_unlink: ");
		}
	}

	sem_unlink(SHARED_MEMORY_MUTEX);

	if ((sharedMemoryMutex = sem_open(SHARED_MEMORY_MUTEX,
	                                  O_CREAT | O_EXCL,
	                                  SHARED_MEMORY_PERMISSIONS,
	                                  1))
	    == SEM_FAILED) {
		HANDLE_ERROR("sem_open: ");
	}

	printDebug(stdout,
	           DEBUG_OK,
	           "Created Shared memory mutex: %s\n",
	           SHARED_MEMORY_MUTEX);
}
/**
 * Cria uma região de memória compartilhada
 * @param size      O tamanho da região de memória compartilhada
 * @param zeroFill  Indica se a memória deve ser preenchida com zeros
 * @return O identificador da região de memória compartilhada
 */
int createSharedMemory(const size_t size, const bool zeroFill)
{
	int shmid;

	initSharedMemoryMutex();

	if ((shmid = shmget(IPC_PRIVATE,
	                    size,
	                    SHARED_MEMORY_PERMISSIONS | IPC_CREAT))
	    < 0) {
		HANDLE_ERROR("shmget: ");
	}

	if (zeroFill) {
		detachSharedMemory(memset(attachSharedMemory(shmid), 0, size));
		printDebug(stdout,
		           DEBUG_INFO,
		           "Zero-filled Shared memory id: %d\n",
		           shmid);
	}

	printDebug(stdout, DEBUG_OK, "Created Shared memory id: %d\n", shmid);

	return shmid;
}
/**
 * Anexa uma região de memória compartilhada ao espaço de endereçamento do processo
 * @param id O identificador da região de memória compartilhada
 * @return Um ponteiro para a região de memória compartilhada
 */
void *attachSharedMemory(const int id)
{
	void *const shm = shmat(id, NULL, 0);

	if (shm == NULL) {
		HANDLE_ERROR("shmat: ");
	}

	printDebug(stdout, DEBUG_OK, "Attached Shared memory id: %d\n", id);

	return shm;
}
/**
 * Desanexa uma região de memória compartilhada do espaço de endereçamento do processo
 * @param sharedMemoryPointer O ponteiro para a região de memória compartilhada
 */
void detachSharedMemory(const void *sharedMemoryPointer)
{
	if (shmdt(sharedMemoryPointer) < 0) {
		HANDLE_ERROR("shmdt: ");
	}
}
/**
 * Deleta uma região de memória compartilhada
 * @param id O identificador da região de memória compartilhada a ser deletada.
 */
void deleteSharedMemory(const int id)
{
	if (shmctl(id, IPC_RMID, NULL) < 0) {
		HANDLE_ERROR("shmctl: ");
	}

	printDebug(stdout, DEBUG_OK, "Deleted Shared memory id: %d\n", id);
}
