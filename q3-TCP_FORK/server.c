#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT_DEFAULT 8080

int main(){
    char ip_name[] = "127.0.0.1";
	int sockfd, response;
	struct sockaddr_in server_address;

	int newSocket;
	struct sockaddr_in new_address;

	socklen_t address_size;

	char buffer[2048];
	pid_t childpid;

    printf("======= Servidor TCP-FORK ======\n");
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("Error ao conectar socket.\n");
		exit(1);
	}
	printf("O Socket foi criado com sucesso!\n");

	memset(&server_address, '\0', sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT_DEFAULT);
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	response = bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address));
	if(response < 0){
		printf("Erro ao tentar fazer o bind\n");
		exit(1);
	}

	printf("Bind feito na porta %d\n", PORT_DEFAULT);

	if(listen(sockfd, 10) == 0){
		printf("Escutando....\n");
	}else{
		printf("Erro ao tentar fazer o bind\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&new_address, &address_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Conexão aceita pelo %s:%d\n", inet_ntoa(new_address.sin_addr), ntohs(new_address.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);

			while(1){
				recv(newSocket, buffer, 1024, 0);
				if(strcmp(buffer, ":exit") == 0){
					printf("Desconectado de %s:%d\n", inet_ntoa(new_address.sin_addr), ntohs(new_address.sin_port));
					break;
				}else{
					printf("Cliente %s:%d: %s\n", inet_ntoa(new_address.sin_addr), ntohs(new_address.sin_port), buffer);
					send(newSocket, buffer, strlen(buffer), 0);
					bzero(buffer, sizeof(buffer));
				}
			}
		}

	}

	close(newSocket);


	return 0;
}