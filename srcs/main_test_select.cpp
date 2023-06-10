
// Server side C program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <vector>

#define PORT1 8080
#define PORT2 8082
int main(int argc, char const *argv[])
{
    (void)argv;
    (void)argc;
    int server_fd1, server_fd2, new_socket;
    long valread;
    struct sockaddr_in address1;
    struct sockaddr_in address2;

    char hello[] = "HTTP/1.0 200 OK\r\n"
                   "Server: webserver-c\r\n"
                   "Content-type: text/html\r\n\r\n"
                   "<h1>hello from server</h1>\r\n";

    // Creating socket file descriptor
    if ((server_fd1 = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    if ((server_fd2 = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address1.sin_family = AF_INET;
    address1.sin_addr.s_addr = INADDR_ANY;
    address1.sin_port = htons(PORT1);

    address2.sin_family = AF_INET;
    address2.sin_addr.s_addr = INADDR_ANY;
    address2.sin_port = htons(PORT2);

    memset(address1.sin_zero, '\0', sizeof(address1.sin_zero));
    memset(address2.sin_zero, '\0', sizeof(address2.sin_zero));

    if (bind(server_fd1, (struct sockaddr *)&address1, sizeof(address1)) < 0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (bind(server_fd2, (struct sockaddr *)&address2, sizeof(address2)) < 0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd1, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd2, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    fd_set  readFdset, writeFdSet;
    FD_ZERO(&readFdset);
    FD_ZERO(&writeFdSet);

    FD_SET(server_fd1, &readFdset);
    FD_SET(server_fd2, &readFdset);
    FD_SET(server_fd1, &writeFdSet);
    FD_SET(server_fd2, &writeFdSet);
    std::vector<int> vecSocket;
    std::vector<int> clientSocket;
    vecSocket.push_back(server_fd1);
    vecSocket.push_back(server_fd2);
    while (1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if (select(vecSocket.size() + 10, &readFdset, &writeFdSet, NULL, NULL) == -1)
        {
            perror("select");
            exit(1);
        }
        for (size_t i = 0; i < vecSocket.size(); i++)
        {
            if (FD_ISSET(vecSocket[i], &readFdset))
            {
                if ((new_socket = accept(vecSocket[i], NULL, NULL)) < 0)
                {
                    perror("In accept");
                    exit(EXIT_FAILURE);
                }
                FD_SET(new_socket, &readFdset);
                FD_SET(new_socket, &writeFdSet);
                clientSocket.push_back(new_socket);
            }
        }
        for (size_t i = 0; i < clientSocket.size(); i++)
        {
            if (FD_ISSET(clientSocket[i], &readFdset))
            {
                char buffer[30000] = {0};
                valread = read(new_socket, buffer, 30000);
                if (valread < 0)
                {
                    perror("read");
                    close(new_socket);
                    FD_CLR(new_socket, &readFdset);
                    exit(1);
                }
                else if (!valread)
                {
                    printf("------------------------Client disconnected!-----------------------------\n");
                    FD_CLR(new_socket, &readFdset);
                    close(new_socket);
                }
                else
                    printf("%s\n", buffer);
            }
            if (FD_ISSET(new_socket, &writeFdSet))
            {
                write(new_socket, hello, strlen(hello));
                printf("------------------Hello message sent-------------------\n");
                close(new_socket);
            }
        }
    }
    std::all_of(vecSocket.begin(), vecSocket.end(), close);
    return 0;
}