/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test_poll.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 19:15:44 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/10 16:12:48 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>

const int BUFFER_SIZE = 1024;
#define PORT 8080

int main() {
    
    int server_fd, newSocket;
    struct sockaddr_in address;
    std::vector<struct pollfd> pollFds;
    std::vector<int> clientSockets;
    std::vector<int> listenSockets;

    char hello[] = "HTTP/1.0 200 OK\r\n"
                   "Server: webserver-c\r\n"
                   "Content-type: text/html\r\n\r\n"
                   "<h1>hello from server</h1>\r\n";

    // Creating socket file descriptor
    for (size_t i = 0; i < 10 ;i++)
    {
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("In socket");
            exit(EXIT_FAILURE);
        }
        listenSockets.push_back(server_fd);
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT + i);

        memset(address.sin_zero, '\0', sizeof(address.sin_zero));

        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        {
            perror("In bind");
            exit(EXIT_FAILURE);
        }
        if (listen(server_fd, 10) < 0)
        {
            perror("In listen");
            exit(EXIT_FAILURE);
        }


        struct pollfd pollFd;
        pollFd.fd = server_fd;
        pollFd.events = POLLIN;
        pollFds.push_back(pollFd);
    }


    while (true) {
        int result = poll(&pollFds[0], pollFds.size(), -1);
        if (result == -1) {
            perror("poll");
            exit(EXIT_FAILURE);
        }

        for (size_t i = 0; i < listenSockets.size(); i++)
        {
            if (pollFds[i].revents & POLLIN) {
                newSocket = accept(pollFds[i].fd, NULL, NULL);
                if (newSocket == -1) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }

                std::cout << "New client connected" << std::endl;

                // Add the new client socket to the pollfds vector
                struct pollfd clientPollFd;
                clientPollFd.fd = newSocket;
                clientPollFd.events = POLLIN;
                pollFds.push_back(clientPollFd);
                clientSockets.push_back(newSocket);
            }
        }

        // Check for read and write events on client sockets
        for (size_t j = listenSockets.size(); j < pollFds.size(); j++) {
            if (pollFds[j].revents & POLLIN) {
                std::cout << "pollFds[" << j << "].fd = " << pollFds[j].fd << std::endl;
                char buffer[BUFFER_SIZE];
                ssize_t bytesRead = read(pollFds[j].fd, buffer, sizeof(buffer));
                if (bytesRead == -1) {
                    perror("read");
                    close(pollFds[j].fd);
                    pollFds.erase(pollFds.begin() + j);
                    clientSockets.erase(clientSockets.begin() + j - 1);
                    exit(EXIT_FAILURE);
                } else if (bytesRead == 0) {
                    // Client disconnected
                    std::cout << "Client disconnected" << std::endl;
                    pollFds[j].events = POLLOUT;
                } else {
                    // Process the received data
                    std::string receivedData(buffer, bytesRead);
                    std::cout << "Received: " << receivedData << std::endl;
                    pollFds[j].events = POLLOUT;
                }
            }
            if (pollFds[j].revents & POLLOUT)
            {
                ssize_t bytesWritten = write(pollFds[j].fd, hello, strlen(hello));
                if (bytesWritten == -1) {
                    perror("Error writing to client");
                    exit(1);
                }
                else if ((size_t)bytesWritten == strlen(hello)){
                    printf("------------------Hello message sent-------------------\n");
                    close(pollFds[j].fd);
                    pollFds.erase(pollFds.begin() + j);
                    clientSockets.erase(clientSockets.begin() + j - listenSockets.size()); // 2 erpresent size of server arrays Don't forget to edit it
                    --j;
                }
            }
        }
    }
    for (size_t i = 0; i < listenSockets.size(); i++)
        close(pollFds[i].fd);

    return 0;
}
