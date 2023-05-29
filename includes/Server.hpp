/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 18:22:19 by lelhlami          #+#    #+#             */
/*   Updated: 2023/05/29 16:52:52 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

const std::string validKeys[] = {"server_name", "listen", "error_page", "client_body_size_max"};
const std::string validLocationKeys[] = {"allow_methods", "redirect", "root", "autoindex", "default", "cgi", "upload"};
typedef short StatusNbr;
typedef std::string Path;
typedef std::string Extension;
typedef std::unordered_map<Extension, Path> CgiPair;
typedef int Socket;
static short Brackets[2] = {0, 0};

class Server
{
public:
	// public attributes
	Socket socketFd;
	std::string hostname;
	std::string port;

	// Location class
	class Location
	{
	public:
		Location();
		Path route;
		// http methods
		bool get;
		bool post;
		bool del;
		// end of http methods
		std::pair<StatusNbr, Path> redirection;
		Path root;
		bool autoindex;
		Path defaultFile;
		Path uploadRoute;
		CgiPair cgi;
		~Location();
	};
	Server();
	void setSettings(std::string &s1, std::string &s2);
	void setServerLocations(std::pair<Path, Location> s1);
	std::unordered_map<std::string, std::string> getSettings() const;
	std::unordered_map<Path, Location> getServerLocations() const;
	~Server();

private:
	std::unordered_map<std::string, std::string> settings;
	std::unordered_map<Path, Location> serverLocations;
};
