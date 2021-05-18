#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

int sockfd, portno, n;
struct sockaddr_in serv_addr;
struct hostent *server;
pthread_t send_thread, receive_thread;
int port = 5000;
char buffer[256];
char bufferReceive[256];
char ip[16];
int option = -1;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void displayMenu() {
	printf("\n========= Cliente TCP =======\n");
	printf("1) Enviar Mensagem\n");
	printf("0) Encerrar programa\n");
}

void *sendMessage() {
	while (option != 0)
	{
		displayMenu();
		scanf("%d", &option);

		switch (option)
		{
			case 1:
				getchar();
				printf("\nDigite a mensagem que deseja enviar: \n");
				bzero(buffer,256);
				fgets(buffer,255,stdin);

				n = write(sockfd, buffer, strlen(buffer));
				if (n < 0) 
					error("ERROR ao enviar mensagem através do socket");
				break;
			case 0:
				break;
			default:
				printf("\nComando não reconhecido, tente novamente!\n");
				break;
		}
	}

	pthread_cancel(send_thread);
	pthread_cancel(receive_thread);
	exit(0);
}

void *receiveMessage() {
	while (1)
	{
		bzero(bufferReceive, 256);
		n = read(sockfd, bufferReceive, 255);
		if (n < 0) 
			error("ERROR ao tentar ler do socket");
		printf("\n\nMensagem Recebida: %s\n", bufferReceive);
	}
}

int main(int argc, char *argv[])
{
	printf("\n========= Cliente TCP =======\n");

	printf("\nDigite o IP desejado:\n");
	bzero(ip,sizeof(ip));
    fgets(ip, sizeof(ip), stdin); 

	printf("\nDigite a porta que deseja usar:\n");
	scanf("%d", &port);

    portno = port;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
	if (sockfd < 0) 
        error("ERROR ao abrir o socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ip);
    
    serv_addr.sin_port = htons(portno);
    
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    pthread_create(&send_thread, NULL, sendMessage, NULL);
    pthread_create(&receive_thread, NULL, receiveMessage, NULL);

    pthread_join(receive_thread, NULL);
    pthread_join(send_thread, NULL);
    
    close(sockfd);
    return 0;
}