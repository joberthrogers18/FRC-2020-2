#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>

int server_instance;
int clients_registered[32];
fd_set read_fd_set;

void setup_client(int client_socket, int index);
static void handleSIGINT();
int get_max_number_between(int a, int b);
int add_client_fd(int client_fd);
void setup_server();

int main()
{
    int port = 8080;

    signal(SIGINT, handleSIGINT);

    server_instance = socket(AF_INET, SOCK_STREAM, 0);
    if (server_instance < 0)
    {
        printf("Erro ao criar o socket: (%s)\n", strerror(errno));
        exit(1);
    }

    printf("\n======= Servidor TCP com Select =======\n");

    // setup server 
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;              
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    if (bind(server_instance, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        printf("Erro ao tentar fazer binding com server: (%s)\n", strerror(errno));
        exit(1);
    }

    FD_ZERO(&read_fd_set);

    // set clients_registered to default value
    for (int i = 0; i < 32; i++)
    {
        clients_registered[i] = -1;
    }

    // start a queue and listen clients
    int is_listening = listen(server_instance, 10);
    if (is_listening < 0)
    {
        printf("Erro ao tentar fazer o listening: (%s)\n", strerror(errno));
        exit(1);
    }

    printf("Servidor executando com sucesso!!\n\n");

    while (1)
    {
        // reset the variable of socket and add the socket server
        FD_ZERO(&read_fd_set);
        FD_SET(server_instance, &read_fd_set);

        // adds valid clients to read_fd_set and finds max_value_to_select
        int max_value_to_select = 0;
        for (int i = 0; i < 32; i++)
        {
            if (clients_registered[i] > 0)
            {
                FD_SET(clients_registered[i], &read_fd_set);
                max_value_to_select = get_max_number_between(max_value_to_select, clients_registered[i]);
            }
        }
        max_value_to_select = get_max_number_between(max_value_to_select, server_instance) + 1;


        // select to handle the multiple clients
        if (select(max_value_to_select, &read_fd_set, NULL, NULL, NULL) < 0)
        {
            printf("Erro no select: (%s)\n", strerror(errno));
            exit(1);
        }

        if (FD_ISSET(server_instance, &read_fd_set))
        {
            setup_server();
        }

        for (int i = 0; i < 32; i++)
        {
            int client_socket_instance = clients_registered[i];

            if (client_socket_instance == -1)
                continue;

            if (FD_ISSET(client_socket_instance, &read_fd_set))
            {
                setup_client(client_socket_instance, i);
            }
        }
    }

    // end comunication with clients
    for (int i = 0; i < 32; i++)
    {
        int socket_fd = clients_registered[i];
        if (socket_fd > -1)
        {
            close(socket_fd);
        }
    }

    close(server_instance);

    return 0;
}

static void handleSIGINT()
{
    FD_ZERO(&read_fd_set);

    for (int i = 0; i < 32; i++)
    {
        int socket_fd = clients_registered[i];
        if (socket_fd > -1)
        {
            close(socket_fd);
        }
    }

    close(server_instance);

    printf("Conexão encerrada\n");
    
    exit(0);
}

int get_max_number_between(int a, int b)
{
    return (a > b) ? a : b;
}

int add_client_fd(int client_fd)
{

    int i;
    for (i = 0; i < 32; i++)
    {
        if (clients_registered[i] == -1)
        {
            clients_registered[i] = client_fd;
            break;
        }
    }

    if (i == 32)
    {
        return -1;
    }

    return i;
}

void setup_server()
{
    struct sockaddr_in client_addr;
    unsigned int client_addr_len = sizeof(client_addr);
    memset(&client_addr, 0, sizeof(client_addr));

    int client_socket_instance = accept(
        server_instance,
        (struct sockaddr *)&client_addr,
        &client_addr_len);

    if (add_client_fd(client_socket_instance) == -1)
    {
        printf("Não foi possível adicionar mais um cliente@\n");
        exit(1);
    }

    printf("Cliente Adicionado com o id: %d\n", client_socket_instance);
}

void setup_client(int client_socket, int index)
{
    int MAX_BUFFER_SIZE = 2048;

    char buffer[MAX_BUFFER_SIZE * 2];

    int result_buffer = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);

    if (result_buffer < 1)
    {
        printf("O Cliente com o id %d fechou com a conexão!\n", client_socket);
        close(client_socket);
        clients_registered[index] = -1;
    }

    buffer[result_buffer] = '\0';

    printf("Mensagem do Cliente %d: %s", client_socket, buffer);

    for (int i = 0; i < 32; i++)
    {
        if (clients_registered[i] == -1 || clients_registered[i] == client_socket)
            continue;

        send(clients_registered[i], buffer, result_buffer, 0);
    }
}
