/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 16:25:26 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/11 15:13:30 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Client.hpp>

Client::Client()
{
    myRequest = new request();
}

Client::~Client()
{
}

void    Client::processRequest()
{
    if ((myBuffer.find("\r\n\r\n") != std::string::npos) && myRequest->header.empty())
    {
        myRequest->header = myBuffer.substr(0, myBuffer.find("\r\n\r\n") + 4);
        myRequest->body = myBuffer.substr(myBuffer.find("\r\n\r\n") + 2);
    }
    else
         myRequest->body += myBuffer;
    if (myRequest->body.find("\r\n0\r\n") != std::string::npos)
    {
        std::ofstream nn;
        nn.open("txt.txt");
        nn << myRequest->body;
        myStage = RESPONSE;
    }
    std::cout << "-----------Header: " << myRequest->header << 	std::endl;
    std::cout << "-----------Body: " << myRequest->body << 	std::endl;
}
