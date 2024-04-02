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

#include "MonitorEngine.h"
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

//não sei onde vai estar...
typedef struct {
    int userID; 
    int dataUsage;
    int dataMax;
    int lastChekpoint; 
} MobileUser;

void sendMessageQueue(char* message, int userID) {
    //aceder a ao id da fila de mensagens 
    //...

}


void checkDataUsage(MobileUser *user) {
    if (user.data_usage >= 0.8 * user.data_limit && user->lastChekpoint < 80) {
        sendMessageQueue("Utilizou 80%% dos dados", user.queue_id);
        logMessage("ALERT 80%% (USER %d) TRIGGERED", user->userID); 
        user->lastChekpoint = 80;
    } else if (user.data_usage >= 0.9 * user.data_limit && user->lastChekpoint < 90) {
        sendMessageQueue("Utilizou 90%% dos dados", user.queue_id);
        logMessage("ALERT 90%% (USER %d) TRIGGERED", user->userID); 
        user->lastChekpoint = 90;
    } else if (user.data_usage >= user.data_limit && user->lastChekpoint < 100) {
        sendMessageQueue("Utilizou 100%% dos dados", user.queue_id);
        logMessage("ALERT 100%% (USER %d) TRIGGERED", user->userID); 
        user->lastChekpoint = 100;
    }
}


void generateStats(MobileUser *user) {
    int percentage = (user->dataUsage * 100) / user->dataMax;
    sendMessageQueue("Atingiu %d%% do limite do seu plano\n", percentage); 
}

void monitorEngine(void)
{
	logMessage(LOG_MONITOR_ENGINE_PROCESS_CREATED);
	sleep(1);
}


void monitorEngine(const int sharedMemoryID) {
    monitorEngine();
    MobileUser *user;
	if ((user = shmat(sharedMemoryID, NULL, 0)) == (MobileUser *) -1) {
		perror("IPC error: shmat");
		exit(EXIT_FAILURE);
	}

    while (true) {
        checkDataUsage(user);
        generateStats(user);
        sleep(30);
    }

	if (shmdt(user) == -1) {
		perror("IPC error: shmdt");
		exit(EXIT_FAILURE);
	}

    return 0;
}
