/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 18:22:04 by lelhlami          #+#    #+#             */
/*   Updated: 2023/05/27 11:50:15 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() {}

Server::~Server() {}

// Location constructor initialisation
Server::Location::Location() : get(false), post(false), del(false), autoindex(false) {}

void Server::setSettings(std::string &s1, std::string &s2)
{
    settings[s1] = s2;
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

Server::Location::~Location() {}