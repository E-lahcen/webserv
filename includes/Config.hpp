/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 16:53:30 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/21 01:24:32 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>

#include <Server.hpp>

class Config
{
public:
	std::vector<Server>		configServers;

	Config(const char *filePath);
	Server::Location getFromLocation(const Path &path) const;

	~Config();

private:
	// Methods
	void load(const char *);
	bool isValidKey(const std::string &key) const;
	bool isValidLocationKey(const std::string &key) const;
	bool isValidBrackets() const;
	std::string trim_spaces(const std::string &str);

	// Setters
	bool setSyntax(std::string&, bool&, bool&);

	// Parsers
	std::pair<Path, Server::Location> parseLocation(std::ifstream &ifile, std::string &line);
	std::pair<StatusNbr, Path> parseRedirection(const std::string &value);
	void parseMethods(Server::Location &, const std::string &);
	std::pair<Extension, Path> parseCgi(const std::string &);
	FilePaths   parseDefaultFile( std::string& );
	void	createDefaultServer();
	bool    checkRequiredKeys();
	void    checkControl();

};


