#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<arpa/inet.h>
#include<sys/socket.h>
#include <unistd.h>
 
// #define IP_SERVER "127.0.0.1"
#define SIZE 2048
// #define PORT 8080   // PORT

void printMenu() {
  printf("======= Cliente Socket =======\n");
  printf("1) Mandar mensagem:\n");
  printf("0) Sair do programa\n");
}

int main(void)
{
    struct sockaddr_in si_other;
    int socket_client;
    int  i, rc;
    socklen_t slen = sizeof(si_other);
    char buffer[SIZE];
    char message[SIZE];
    int option = -1;
    char ip_server[16];
    int port = 8080;

    
    printf("====== Cliente UDP ======\n\n");
    printf("\nDigite o IP desejado:\n");
	bzero(ip_server, sizeof(ip_server));
    fgets(ip_server, sizeof(ip_server), stdin); 

    printf("\nDigite a porta do servidor:\n");
	scanf("%d", &port);
 
    // define the udp transmition setup
    if ((socket_client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        perror("Erro na abertura do socket");
        exit(1);
    }
    
    // Configurando a estrutura de dados sockaddr_in
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(port);
     
    if (inet_aton(ip_server , &si_other.sin_addr) == 0) 
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
                
                if (sendto(socket_client, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
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
         
        memset(buffer,'\0', SIZE); 

        if (recvfrom(socket_client, buffer, SIZE, 0, (struct sockaddr *) &si_other, &slen) == -1)
        {
            fprintf(stderr, "Erro no recvfrom()\n");
        }
        puts(buffer);
    }
 
    close(socket_client);
    return 0;
}
