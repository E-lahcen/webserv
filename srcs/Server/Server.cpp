/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydahni <ydahni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 18:22:04 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/11 17:44:38 by ydahni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

Server::Server() {}

Server::~Server() {}

// Location constructor initialisation
Server::Location::Location() : get(false), post(false), del(false), autoindex(false) {}

void Server::setSettings(std::string &key, std::string &value)
{
    if (key == "server_name")
        serverName = value;
    if (key == "listen")
        parseHostnamePort(value);
    else if (key == "error_page")
        parseErrorPage(value);
    else if (key == "client_body_size_max")
        setClientBodySizeMax(value);
}

void Server::setServerLocations(std::pair<Path, Location> s1)
{
    serverLocations.insert(s1);
}

std::unordered_map<Path, Server::Location> Server::getServerLocations() const
{
    return serverLocations;
}

size_t Server::getBodySizeMax(void)
{
    return clientBodySizeMax;
}

void Server::setClientBodySizeMax(std::string &value)
{
    if (!is_number(value))
        throw std::runtime_error("Invalid Client Body Size Max input : " + value);

    std::stringstream ss;

    ss << value;
    ss >> clientBodySizeMax;
    ss.clear();
}

void Server::parseHostnamePort(std::string &value)
{
    std::size_t colonPos = value.find(':');
    std::string tmpPort;

    if (colonPos != std::string::npos)
    {
        hostname = value.substr(0, colonPos);
        tmpPort = value.substr(colonPos + 1);
        (is_number(tmpPort) && std::atoi(tmpPort.c_str()) <= MAX_PORT_NUMBER) ? port = tmpPort : throw std::runtime_error("Invalid port format in configuration file!");
    }
    else
        throw std::runtime_error("Invalid hostname:port format in configuration file!");
}

void Server::parseErrorPage(std::string &value)
{
    std::stringstream ss(value);
    StatusNbr tmpStat;
    Path tmpPath;
    std::string tmp;

    ss >> tmp;
    (is_number(tmp) && tmp.length() == 3) ? tmpStat = std::atoi(tmp.c_str()) : throw std::runtime_error("Invalid status error number!");
    ss >> tmpPath;
    errorPage[tmpStat] = tmpPath;
}

Server::Location::~Location() {}