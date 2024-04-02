#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/msg.h>



void send_message(int user_id, const char *service_id, int data_reservation) {}

void sigint_handler(int signum) {
    printf("Received SIGINT. Exiting...\n");
    exit(EXIT_SUCCESS);
}

int main() {
    signal(SIGINT, sigint_handler);
    
    //implementar "O identificador do BackOffice User a utilizar é 1. "

    int data_stats = 0;
    int reset_stats = 0;

    while (1) {
        char command[50];
        scanf("%s", command);

        if (strcmp(command, "data_stats") == 0) {
            data_stats = 1;
        } else if (strcmp(command, "reset") == 0) {
            reset_stats = 1;
        } else {
            printf("Invalid command. Use data_stats or reset.\n");
            continue;
        }

        if (data_stats == 1) {
            printf("Service\t\tTotal Data\tAuth Reqs\n");
            //receive()...
            data_stats = 0;
        }

        if (reset_stats == 1) {
            //reset()
            printf("Statistics cleared.\n");
            reset_stats = 0;
        }
    }

    return 0;
}
