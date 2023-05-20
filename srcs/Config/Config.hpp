/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 16:53:30 by lelhlami          #+#    #+#             */
/*   Updated: 2023/05/19 14:39:48 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <fstream>
#include <unordered_map>

const std::string validKeys[] = { "server_name", "listen", "error_page", "client_body_size_max" };
const std::string validLocationKeys[] = { "allow_methods", "redirect", "root", "autoindex", "default", "cgi", "upload"};

class Config {
    public:
        Config(const std::string& filePath);
        std::string get(const std::string& key) const;
        ~Config();
    private:
        void    load( const std::string& );
        std::unordered_map<std::string, std::string> settings;
        
        bool isValidKey(const std::string& key) const;

};

std::string trim_spaces( const std::string&   str );

#endif
