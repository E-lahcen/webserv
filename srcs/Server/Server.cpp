/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 18:22:04 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/21 20:40:22 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

Server::Server()
{
    const std::string	requiredKeys[] = {"server", "listen", "client_body_size_max", "location"};
    std::list<std::string>	copyRequiredKeysList(requiredKeys, requiredKeys + sizeof(requiredKeys) / sizeof(requiredKeys[0]));
    requiredKeysList = copyRequiredKeysList;
}

Server::~Server() {}

// Location constructor initialisation
Server::Location::Location() : get(false), post(false), del(false), autoindex(false) 
{
    const std::string	requiredKeys[] = {"allow_methods", "root"};
    std::list<std::string>	copyRequiredLocationKeysList(requiredKeys, requiredKeys + sizeof(requiredKeys) / sizeof(requiredKeys[0]));
    requiredLocationKeysList = copyRequiredLocationKeysList;
}

void Server::setSettings(std::vector<std::string>& listServerNames, std::string &key, std::string &value)
{
    if (key == "server_name")
    {
        if (std::find(listServerNames.begin(), listServerNames.end(), value) != listServerNames.end())
            throw std::runtime_error("Invalid configuration : Duplicate Name servers");
        listServerNames.push_back(value);
        serverName = value;
    }
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
    if (!is_number(value) || value.size() > 9)
        throw std::runtime_error("Invalid Client Body Size Max input : " + value);

    std::stringstream ss;

    ss << value;
    ss >> clientBodySizeMax;
    if (clientBodySizeMax > 100000000)
    {
        ss.clear();
        throw std::runtime_error("Invalid Client Body Size Max input : " + value);
    }
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