#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <signal.h>

#define ID_VIDEO VIDEO
#define ID_MUSIC MUSIC 
#define ID_SOCIAL SOCIAL 

void send_message(int user_id, const char *service_id, int data_reservation) {}

void sigint_handler(int signum) {
    printf("Received SIGINT. Exiting...\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    if (argc != 7) {
        fprintf(stderr, "Usage: %s plafond_inicial num_pedidos  intervalo_video intervalo_music intervalo_social dados_reservar\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, sigint_handler);

    int plafond_inicial = atoi(argv[1]);
    int num_pedidos = atoi(argv[2]);
    int intervalo_video = atoi(argv[3]);
    int intervalo_music = atoi(argv[4]);
    int intervalo_social = atoi(argv[5]);
    int dados_reservar = atoi(argv[6]);

    int authorization_requests = 0; 
    while (1) {
        //implementar condiçoes de saida
        /**
         *  1 Receção de um sinal SIGINT; 
            2. Receção de um alerta de 100% relativo ao plafond de dados; 
            3. No caso de o número máximo de pedidos de autorização ser atingido; 
            4. Em caso de erro - um erro pode acontecer se algum parâmetro estiver errado ou ao tentar 
                escrever para o named pipe e a escrita falhar. Nestes casos deve escrever a mensagem de erro 
                no ecrã. 
        */
        if (authorization_requests > num_pedidos) break; 

        send_message(user_id, ID_VIDEO, dados_reservar);
        authorization_requests++;

        send_message(user_id, ID_MUSIC, dados_reservar);
        authorization_requests++;

        send_message(user_id, ID_SOCIAL, dados_reservar);
        authorization_requests++;
    }

    return 0;
}