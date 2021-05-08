/*
    Exemplo de Servidor UDP    
*/
#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
 
#define TAMANHO 1024  
#define PORTA 8080  

int option = -1;

void printMenu() {
  printf("======= Server Socket =======\n");
  printf("1) Mandar mensagem:\n");
  printf("0) Sair do programa\n");
} 

int main(void)
{
    struct sockaddr_in si_me, si_other;
    int socket_servidor;
    int s, i, recv_len;
    socklen_t slen = sizeof(si_other);
    char buffer[TAMANHO];
    char message[TAMANHO];
     
    if ((socket_servidor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        printf("Erro na abertura do socket\n");
        exit(1);
    }
     
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORTA);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    if( bind(socket_servidor , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        printf("Erro no bind\n");
        exit(1);
    }
     
    while(option != 0)
    {         
        if ((recv_len = recvfrom(socket_servidor, buffer, TAMANHO, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            printf("Erro no recvfrom()\n");
            exit(1);
        }
         
        printf("Pacote recebido de %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Mensagem: %s\n" , buffer);
         
        printMenu();
        scanf("%d", &option);

        switch (option)
        {
            case 1:
                    printf("\nMensagem a ser enviada : ");
                    scanf("%s", message);

                    if (sendto(socket_servidor, message, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
                    {
                        printf("Erro no sendto()\n");
                        exit(1);
                    }
                break;
            case 0:
                break;
            default:
                printf("Opção errada, Digite Novemente");
                break;
        }
    }
 
    close(socket_servidor);
    return 0;
}
