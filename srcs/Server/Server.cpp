/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 18:22:04 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/04 11:35:33 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() {}

Server::~Server() {}

// Location constructor initialisation
Server::Location::Location() : get(false), post(false), del(false), autoindex(false) {}

void Server::setSettings(std::string &key, std::string &value)
{
    settings[key] = value;
    if ( key == "listen" )
        parseHostnamePort(value);
    if ( key == "error_page" )
        parseErrorPage(value);
}

void Server::setServerLocations(std::pair<Path, Location> s1)
{
    serverLocations.insert(s1);
}

std::unordered_map<std::string, std::string> Server::getSettings() const
{
    return settings;
}

std::unordered_map<Path, Server::Location> Server::getServerLocations() const
{
    return serverLocations;
}

void    Server::parseHostnamePort( std::string& value )
{
    std::size_t colonPos = value.find(':');
    if (colonPos != std::string::npos)
    {
        hostname = value.substr(0, colonPos);
        port = value.substr(colonPos + 1);
    }
    else
        std::runtime_error("Invalid hostname:port format in configuration file!");
}

void    Server::parseErrorPage( std::string& value )
{
    std::stringstream   ss(value);

    ss >> errorPage.first;
    ss >> errorPage.second;
}

Server::Location::~Location() {}