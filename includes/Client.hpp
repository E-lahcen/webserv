/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrahali <nrahali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 16:23:53 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/14 13:45:15 by nrahali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Network.hpp>
#include <request.hpp>
#include <Response.hpp>

typedef std::vector<Server> Servers;


enum Level {
    REQUEST,
    RESPONSE,
    FINISH
};

class request;
class Response;

class   Client
{
    public:
        Client();  
        ~Client();
        
        void    processRequest( Servers& servers );
        void    processResponse();
        
        request*     myRequest;
        Response*     myResponse;
        bool        CloseSocket;
        Socket      mySocket;
        Server      myServer;
        Level       myStage;
        std::string myBuffer;
    private:
};