/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydahni <ydahni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 16:25:26 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/21 23:58:23 by ydahni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Client.hpp>

Client::Client()
{
    myRequest = new request();
    myResponse = new Response();
    this->CloseSocket = false;
}

Client::~Client()
{
}

void    Client::processRequest( Servers& servers )
{
    if ((myBuffer.find("\r\n\r\n") != std::string::npos) && myRequest->header.empty())
    {
        myRequest->header = myBuffer.substr(0, myBuffer.find("\r\n\r\n") + 4);
        myRequest->GetRequest(servers);
        myBuffer = myBuffer.substr(myBuffer.find("\r\n\r\n") + 2);
    }
    if (myRequest->GetStatutCode() != 0 && myRequest->GeterMethod() == "POST")
    {
        myRequest->GetBody(myBuffer);
    }
    if (myRequest->multipart == 1)
    {
        myRequest->GetMultipart(myBuffer);
        myRequest->finishRead = 0;
    }
    if (myRequest->finishRead == 0)
        myStage = RESPONSE;
}




// Begin of Response processing 

void    Client::processResponse()
{
    myResponse->fd = mySocket;
    myResponse->ResponseMessage(*myRequest);
    if (myResponse->Closedfile == true)
        CloseSocket = true;
}