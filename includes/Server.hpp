/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 18:22:19 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/07 16:16:45 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

#define MAX_PORT_NUMBER 65535

const std::string validKeys[] = {"server_name", "listen", "error_page", "client_body_size_max"};
const std::string validLocationKeys[] = {"allow_methods", "redirect", "root", "autoindex", "default", "cgi", "upload"};

typedef short StatusNbr;
typedef std::string Path;
typedef std::string Extension;
typedef std::unordered_map<Extension, Path> CgiPair;
typedef std::pair<StatusNbr, Path> redirectionPair;
typedef std::unordered_map<StatusNbr, Path> errorPagePair;
typedef int Socket;

class Server
{
public:
	// public attributes
	std::string serverName;
	Socket socketFd;
	std::string hostname;
	std::string port;
	size_t clientBodySizeMax;
	errorPagePair errorPage;

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
		redirectionPair redirection;
		Path root;
		bool autoindex;
		Path defaultFile;
		Path uploadRoute;
		CgiPair cgi;
		~Location();
	};
	Server();

	// Getters
	size_t getBodySizeMax(void);
	std::unordered_map<Path, Location> getServerLocations() const;

	// Setters
	void setSettings(std::string &s1, std::string &s2);
	void setServerLocations(std::pair<Path, Location> s1);
	void setClientBodySizeMax(std::string &value);

	// Parsers
	void parseHostnamePort(std::string &);
	void parseErrorPage(std::string &);

	~Server();

private:
	std::unordered_map<Path, Location> serverLocations;
};

#include <utils.hpp>
