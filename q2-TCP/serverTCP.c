/* The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include<signal.h>

int sockfd, newsockfd, portno;
socklen_t clilen;
char buffer[256];
char bufferReceive[256];
struct sockaddr_in serv_addr, cli_addr;
pthread_t send_thread, receive_thread;
int n, port;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void *sendMessage() {
    while (1)
    {
        getchar();
        printf("Digite a mensagem que deseja enviar: \n");
        bzero(buffer,256);
        fgets(buffer,255,stdin);

        // This send() function sends the 13 bytes of the string to the new socket
        send(newsockfd, buffer, strlen(buffer), 0);
    }
}

void *receiveMessage() {
	while (1)
	{
		bzero(bufferReceive,256);

        n = read(newsockfd, bufferReceive, 255);
        if (n < 0) error("ERROR ao ler do socket");
        printf("Mensagem enviada ao servidor: %s\n", bufferReceive);
	}
}

void handle_sigint(int sig)
{
    pthread_cancel(receive_thread);
    pthread_cancel(send_thread);
    close(newsockfd);
    close(sockfd);
    exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, handle_sigint);
    
    printf("========= Servidor TCP =========\n");

    // socket(int domain, int type, int protocol)
    sockfd =  socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR ao abrir o socket");

    // clear address structure
    bzero((char *) &serv_addr, sizeof(serv_addr));

    printf("\nDigite a porta que deseja usar:\n");
    scanf("%d", &port);
    portno = port;

    // configure the server instance
    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_addr.s_addr = INADDR_ANY;  
    serv_addr.sin_port = htons(portno);

    // This bind() call will bind  the socket to the current IP address on port
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0) 
            error("ERROR ao fazer o binding");

    // Set the maximum size for the backlog queue to 5.
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    // communicating with the connected client.
    newsockfd = accept(
        sockfd, 
        (struct sockaddr *) &cli_addr, 
        &clilen
    );

    if (newsockfd < 0) 
        error("ERROR ao aceitar socket");

    pthread_create(&send_thread, NULL, sendMessage, NULL);
    pthread_create(&receive_thread, NULL, receiveMessage, NULL);

    pthread_join(receive_thread, NULL);
    pthread_join(send_thread, NULL);

    return 0; 
}