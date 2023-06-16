/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test_request_config.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 15:20:32 by ydahni            #+#    #+#             */
/*   Updated: 2023/06/16 14:44:38 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Config.hpp>

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <poll.h>

#define PORT 8081
#define MAX_CLIENT 20

// int request::my_main()
// {
//     int server_fd, new_socket = 0;
//     long valread = 0;
//     struct sockaddr_in address;
//     int addrlen = sizeof(address);
//     (void)addrlen;
//     (void)valread;

//     // Only this line has been changed. Everything is same.
//     std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

//     // Creating socket file descriptor
//     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
//     {
//         perror("In socket");
//         exit(EXIT_FAILURE);
//     }

//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(PORT);

//     memset(address.sin_zero, '\0', sizeof address.sin_zero);

//     if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
//     {
//         perror("In bind");
//         exit(EXIT_FAILURE);
//     }
//     if (listen(server_fd, MAX_CLIENT) < 0)
//     {
//         perror("In listen");
//         exit(EXIT_FAILURE);
//     }

//     // Set up the pollfd structure for server socket
//     struct pollfd serverPollFd;
//     serverPollFd.fd = server_fd;
//     serverPollFd.events = POLLIN;

//     // Set up an array of client pollfds
//     std::vector<struct pollfd> clientPollFds(MAX_CLIENT);
//     std::vector<int> clientSockets(MAX_CLIENT, -1);


//     while (1)
//     {
        
//         printf("\n+++++++ Waiting for new connection ++++++++\n\n");
//         // Create an array of pollfds
//         std::vector<struct pollfd> pollFds(1 + clientSockets.size());
//         pollFds[0] = serverPollFd;
//         for (size_t i = 0; i < clientSockets.size(); ++i) {
//             pollFds[i + 1].fd = clientSockets[i];
//             pollFds[i + 1].events = POLLIN;
//         }

//         // Wait for events on the file descriptors
//         if (poll(pollFds.data(), pollFds.size(), -1) == -1) {
//             std::cerr << "Error in poll()\n";
//             break;
//         }
//         // Check for incoming connection on server socket
//         if (pollFds[0].revents & POLLIN) {
//             // Accept the incoming connection
//             struct sockaddr_in clientAddress;
//             socklen_t clientAddressLength = sizeof(clientAddress);
//             int newSocket = accept(server_fd, (struct sockaddr*)&clientAddress, &clientAddressLength);
//             if (newSocket == -1) {
//                 std::cerr << "Failed to accept connection\n";
//                 continue;
//             }

//             std::cout << "New client connected!" << std::endl;

//             // Find an available slot in the client sockets array
//             size_t clientIndex = 0;
//             for (; clientIndex < clientSockets.size(); ++clientIndex) {
//                 if (clientSockets[clientIndex] == -1) {
//                     clientSockets[clientIndex] = newSocket;
//                     break;
//                 }
//             }
//         }
        
//         // Check for data on client sockets
//         for (size_t clientIndex = 0; clientIndex < clientSockets.size(); ++clientIndex) {
//             int clientSocket = clientSockets[clientIndex];
//             if (clientSocket != -1 && pollFds[clientIndex + 1].revents & POLLIN) {
//                 char buffer[1024];
//                 ssize_t bytesRead = read(clientSocket, buffer, sizeof(buffer));
//                 if (bytesRead > 0) {
//                     // Read data from client
//                     std::string data(buffer, bytesRead);
//                     std::cout << "Received data from client: " << data << std::endl;

//                     // Process the received data

//                     // Send a response back to the client
//                     std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nHello";
//                     write(clientSocket, response.c_str(), response.length());
//                     std::cout << "Response sent to client" << std::endl;
                    
//                 } else if (bytesRead == 0) {
//                     // Client disconnected
//                     std::cout << "Client disconnected" << std::endl;
//                     close(clientSocket);
//                     clientSockets[clientIndex] = -1;
//                 } else {
//                     // Error in reading data
//                     std::cerr << "Error in reading data from client" << std::endl;
//                     close(clientSocket);
//                     clientSockets[clientIndex] = -1;
//                 }
//             }
//         }
        
//         // write(new_socket, hello.c_str(), hello.length());
//         // printf("------------------Hello message sent-------------------");
//         close(new_socket);
//     }
//     close(server_fd);
//     return (0);
// }

int request::my_main()
{
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Only this line has been changed. Everything is same.
    std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

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
    while (1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        char buffer[1024] = {0};
        valread = 1;
        std::string b;

        int i = 0;
        while (valread > 0)
        {
            valread = read(new_socket, buffer, 1024);
            if (valread < 0)
            {
                perror("Read");
                exit(EXIT_FAILURE);
            }
            b = std::string(buffer, valread);
            if ((b.find("\r\n\r\n") != std::string::npos) && i == 0)
            {
                i++;
                this->header = b.substr(0, b.find("\r\n\r\n") + 4);
                std::cout << this->header << std::endl;
                b = b.substr(b.find("\r\n\r\n") + 2);
            }
            std::cout << "\n------------------here-------------------\n" << std::endl; 
            this->body += b;
        }
        printf("------------------Hello message sent-------------------");
        if (write(new_socket, hello.c_str(), hello.length()) < 0)
        {
            close(new_socket);
            break;
        }
        close(new_socket);
    }
    return (0);
}

int main(int ac, char **av)
{
    try
    {
        std::srand(std::time(0));
        (void)ac;
        // (void)av;
        // request a;
        Config config(av[1]);
        // a.my_main();
        // a.printHeader();
        // a.GetRequest(config);
        // if (a.GeterMethod() == "GET")
        // {
        //     a.printHeader();
        // }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading configuration: " << e.what() << std::endl;
    }
    // system("leaks webserv");
}