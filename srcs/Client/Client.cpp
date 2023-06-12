/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 16:25:26 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/12 12:37:05 by lelhlami         ###   ########.fr       */
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

void    Client::processRequest( Servers& servers )
{
    if ((myBuffer.find("\r\n\r\n") != std::string::npos) && myRequest->header.empty())
    {
        myRequest->header = myBuffer.substr(0, myBuffer.find("\r\n\r\n") + 4);
        myBuffer = myBuffer.substr(myBuffer.find("\r\n\r\n") + 2);
    }

    if (myBuffer.find("\r\n0\r\n") != std::string::npos)
    {
        myRequest->body = myBuffer;
        myRequest->GetRequest(servers);
        myStage = RESPONSE;
    }
    std::cout << "-----------Header: " << myRequest->header << 	std::endl;
    // std::cout << "-----------Body: " << myRequest->body << 	std::endl;
}
