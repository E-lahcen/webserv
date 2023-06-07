/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test_request_config.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 15:20:32 by ydahni            #+#    #+#             */
/*   Updated: 2023/06/07 18:40:22 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <request.hpp>
#include <Config.hpp>

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080

int request::my_main()
{
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    (void)addrlen;

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
        if ((new_socket = accept(server_fd, NULL, NULL)) < 0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        char buffer[3000] = {0};
        valread = 1;
        std::string b;

        int i = 0;
        while (valread > 0)
        {
            valread = read(new_socket, buffer, 300000);
            b = std::string(buffer, valread);
            if ((b.find("\r\n\r\n") != std::string::npos) && i == 0)
            {
                i++;
                this->header = b.substr(0, b.find("\r\n\r\n") + 4);
                // std::cerr << this->header << std::endl;
                b = b.substr(b.find("\r\n\r\n") + 2);
            }
            this->body += b;
        }
        // std::cerr << this->body << std::endl;
        write(new_socket, hello.c_str(), hello.length());
        printf("------------------Hello message sent-------------------");
        close(new_socket);
        break;
    }
    return (0);
}

int main(int ac, char **av)
{
    try
    {
        (void)ac;
        // (void)av;
        // request a;
        Config config(av[1]);
        // a.my_main();
        // a.printHeader();
        // a.GetRequest(config);
        // if (a.GeterMethod() == "GET")
        // {
        //     // a.printHeader()
        // }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading configuration: " << e.what() << std::endl;
        return 1;
    }
}