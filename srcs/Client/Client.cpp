/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 16:25:26 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/11 13:40:41 by lelhlami         ###   ########.fr       */
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
    if ((myBuffer.find("\r\n\r\n") != std::string::npos))
    {
        myRequest->header = myBuffer.substr(0, myBuffer.find("\r\n\r\n") + 4);
        myRequest->body = myBuffer.substr(myBuffer.find("\r\n\r\n") + 2);
    }
    std::cout << "-----------Header: " << myRequest->header << 	std::endl;
    std::cout << "-----------Body: " << myRequest->body << 	std::endl;
}
