#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>

int pid = 0;
int socket_instance;

void setup_conn_server();
int message_verify_listening();
void connect_socket();

int main()
{
    socket_instance = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_instance < 0)
    {
        printf("socket creation failed (%s)\n", strerror(errno));
        exit(-1);
    }

    printf("\n======= Cliente TCP com Select =======\n\n");

    connect_socket();
    pid = fork();

    // verify if the pid is child or parent
    if (pid != 0)
    {
        while (1)
        {
            if (message_verify_listening() < 1)
            {
                break;
            }
        }

        kill(pid, SIGINT);
    }
    else
    {
        setup_conn_server();
    }

    close(socket_instance);

    return 0;
}

int message_verify_listening()
{
    char buffer[2048];

    int read_bytes = recv(socket_instance, buffer, 2048, 0);

    if (read_bytes == 0)
    {
        printf("Não foi possível recuperar dado\n");
    }
    else if (read_bytes < 0)
    {
        printf("Erro ao tentar conectar: (%s)\n", strerror(errno));
    }
    else
    {
        buffer[read_bytes] = '\0';
        printf("\nMesagem vinda do Servidor: %s\n", buffer);
    }

    return read_bytes;
}

void setup_conn_server()
{
    char buffer[2048];

    while (1)
    {
        // get message to send
        printf("Digite a mensagem que deseja enviar:  ");

        // read buffer to send to server
        char currentCharacter;
        int currentCountBuffer = 0;
        do
        {
            currentCharacter = getchar();
            buffer[currentCountBuffer++] = currentCharacter;
        } while (currentCharacter != '\n');
        buffer[currentCountBuffer++] = '\0';

        if (write(socket_instance, buffer, currentCountBuffer) < 1)
        {
            break;
        }
    }
}

void connect_socket() {
    int port = 8080;
    
    // setup server 
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    int is_connected = connect(socket_instance, (struct sockaddr *)&server_address, sizeof(server_address));

    if (is_connected != 0)
    {
        printf("Erro ao tentar conectar com o servidor: (%s)\n", strerror(errno));
        exit(-1);
    }
}