/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 16:53:30 by lelhlami          #+#    #+#             */
/*   Updated: 2023/05/23 11:05:19 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <fstream>
#include <unordered_map>

const std::string validKeys[] = { "server_name", "listen", "error_page", "client_body_size_max" };
const std::string validLocationKeys[] = { "allow_methods", "redirect", "root", "autoindex", "default", "cgi", "upload"};
typedef short       StatusNbr;
typedef std::string Path;

class Config {
    public:
        Config(const char* filePath);
        std::string get(const std::string& key) const;
        
        //Location class 
        class Location {
            
			Location();

            public:
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
        bool isValidLocationKey(const std::string& key) const;

};

std::string trim_spaces( const std::string&   str );

#endif
