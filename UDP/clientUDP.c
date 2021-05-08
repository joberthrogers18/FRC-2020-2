#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<arpa/inet.h>
#include<sys/socket.h>
#include <unistd.h>
 
#define IP_SERVIDOR "127.0.0.1"
#define TAMANHO 1024  // Tamanho máximo do buffer
#define PORTA 8080   // Porta

void printMenu() {
  printf("======= Cliente Socket =======\n");
  printf("1) Mandar mensagem:\n");
  printf("0) Sair do programa\n");
}


int main(void)
{
    struct sockaddr_in si_other;
    int socket_cliente;
    int  i, rc;
    socklen_t slen = sizeof(si_other);
    char buffer[TAMANHO];
    char message[TAMANHO];
    int option = -1;
 
    if ((socket_cliente = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        perror("Erro na abertura do socket");
        exit(1);
    }
    
    // Configurando a estrutura de dados sockaddr_in
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORTA);
     
    if (inet_aton(IP_SERVIDOR , &si_other.sin_addr) == 0) 
    {
        fprintf(stderr, "Falha no inet_aton()\n");
        exit(1);
    }
 
    while(1)
    {
        printMenu();
        scanf("%d", &option);

        switch (option)
        {
            case 1:
                printf("Mensagem a ser enviada : ");
                scanf("%s", message);
                
                if (sendto(socket_cliente, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
                {
                    fprintf(stderr, "Falha no sendto()\n");
                    exit(1);
                }
                break;
            case 0:
                break;
            default:
                printf("Opção errada, Digite Novemente");
                break;
        }
         
        memset(buffer,'\0', TAMANHO); 

        if (recvfrom(socket_cliente, buffer, TAMANHO, 0, (struct sockaddr *) &si_other, &slen) == -1)
        {
            fprintf(stderr, "Erro no recvfrom()\n");
        }
        puts(buffer);
    }
 
    close(socket_cliente);
    return 0;
}
