/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 16:53:30 by lelhlami          #+#    #+#             */
/*   Updated: 2023/05/17 11:57:39 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

std::string validKeys[] = { "server_name", "listen", "error_page", "client_body_size_max" };

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
