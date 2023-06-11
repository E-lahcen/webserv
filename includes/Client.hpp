/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 16:23:53 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/11 13:28:14 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Network.hpp>
#include <request.hpp>

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
        void    processRequest();
        
        request*     myRequest;
        Socket      mySocket;
        Server      myServer;
        Level       myStage;
        std::string myBuffer;
    private:
};