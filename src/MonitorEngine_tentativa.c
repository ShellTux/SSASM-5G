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
    int dataUsage;
    int dataMax;
} MobileUser;

void sendMessageQueue(char* message) {
    //aceder a ao id da fila de mensagens 
    //...

}

void checkDataUsage(MobileUser *user) {
    int percentage = (user->dataUsage * 100) / user->dataMax;
    if (percentage >= 100) {
        sendMessageQueue("Uso de dados atingiu 100%% do limite.\n"); 
    }else if (percentage >= 90 ) {
        sendMessageQueue("Uso de dados atingiu 90%% do limite.\n"); 
    }else if (percentage >= 80) {
        sendMessageQueue("Uso de dados atingiu 80%% do limite.\n"); 
    }
}


void generateStats(MobileUser *user) {
    int percentage = (user->dataUsage * 100) / user->dataMax;
    sendMessageQueue("Atingiu %d%% do limite do seu plano\n", percentage); 
}


void monitorEngine(const int sharedMemoryID) {
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
