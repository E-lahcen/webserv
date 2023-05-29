/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 16:53:30 by lelhlami          #+#    #+#             */
/*   Updated: 2023/05/28 17:12:45 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Server.hpp>
#include <vector>

class Config
{
public:
	Config(const char *filePath);
	std::string get(const std::string &key) const;
	Server::Location getFromLocation(const Path &path) const;
	~Config();

private:
	std::vector<Server> configServers;

	void load(const char *);
	bool setSyntax(std::string &line);
	bool isValidKey(const std::string &key) const;
	bool isValidLocationKey(const std::string &key) const;
	bool isValidBrackets() const;
	std::string trim_spaces(const std::string &str);
	std::pair<Path, Server::Location> parseLocation(std::ifstream &ifile, std::string &line);
	std::pair<StatusNbr, Path> parseRedirection(const std::string &value);
	void parseMethods(Server::Location &, const std::string &);
	std::pair<Extension, Path> parseCgi(const std::string &);
};
