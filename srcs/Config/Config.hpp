/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 16:53:30 by lelhlami          #+#    #+#             */
/*   Updated: 2023/05/24 00:20:25 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

const std::string validKeys[] = { "server_name", "listen", "error_page", "client_body_size_max" };
const std::string validLocationKeys[] = { "allow_methods", "redirect", "root", "autoindex", "default", "cgi", "upload"};
typedef short           StatusNbr;
typedef std::string     Path;
static short			Brackets[2] = {0, 0};

class Config {
	public:
		Config(const char* filePath);
		std::string get(const std::string& key) const;
		
		//Location class 
		class Location {
			

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
				~Location();
		};
		
		~Config();
	private:
		void    load( const char* );
		std::unordered_map<std::string, std::string>    settings;
		std::unordered_map<Path, Location>              configLocations;
		
		bool isValidKey(const std::string& key) const;
		// bool isValidLocationKey(const std::string& key) const;
		bool isValidBrackets( ) const;
		bool setSyntax( std::string& line) const;

};

std::string 		trim_spaces( const std::string&   str );
std::pair<Path, Config::Location>	parseLocation(const std::string& value);

#endif
