/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 16:23:53 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/10 20:59:03 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Network.hpp>
#include <request.hpp>

class   Server;
// class   request;

enum Level {
    REQUEST,
    RESPONSE,
    FINISH
};

class   Client
{
    public:
        Client();  
        ~Client();
        
        // request myRequest;
        Socket  mySocket;
        Server  myServer;
        Level   myStage;
        std::string myBuffer;
};