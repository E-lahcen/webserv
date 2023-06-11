/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydahni <ydahni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 16:23:53 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/11 17:37:13 by ydahni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Network.hpp>
#include <request.hpp>

typedef std::vector<Server> Servers;


enum Level {
    REQUEST,
    RESPONSE,
    FINISH
};

class request;

class   Client
{
    public:
        Client();  
        ~Client();
        void    processRequest( Servers& servers );
        
        request*     myRequest;
        // response*     myReponse;
        Socket      mySocket;
        Server      myServer;
        Level       myStage;
        std::string myBuffer;
    private:
};