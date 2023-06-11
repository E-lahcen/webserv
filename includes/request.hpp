/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydahni <ydahni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 15:20:13 by ydahni            #+#    #+#             */
/*   Updated: 2023/06/11 16:30:58 by ydahni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Config.hpp>
#include <Network.hpp>

typedef std::vector<Server> Servers;

class Config;

//check if directory
#include <sys/stat.h>

//for request 
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <unistd.h>

class request
{
    private:
        //header
        std::map<std::string, std::string> map;
        std::string method;
        std::string uri;
        std::string version;
        //body
        std::vector<std::pair<std::string, std::string> > Multipart;
        //
        size_t BodySizeMax;
        std::string Rhostname;
	    std::string Rport;
    
        //for respons
        std::string path;
        int StatutCode;
    public:
        std::string header;
        std::string body;
        //for method Get
        std::string GeterMethod() const;
        std::string GeterUri() const;
        std::string GeterVersion() const;
        std::map<std::string, std::string> GeterHeader() const;
        void printHeader() const;
        void SetStatutCode(int statut);
        int GetStatutCode() const;

        //for request resources
        int my_main();
        void GetRequest( Servers& servers );
        void GetBody();
        void Getheader();
        void GetChunked();
        void GetMultipart();

        //
        int CheckLocation(std::string location, std::string root);
        void CheckIfMethodAllowed(std::vector<Server>::iterator it);
        void Post(std::unordered_map<Path, Server::Location>::iterator &itl);
        void upload(std::unordered_map<Path, Server::Location>::iterator &itl);
        void CreatFileInDirectoryOfUpload(std::string &path);

        //for error
        void CheckErrors();


        //for class
        request();
        ~request();
};


std::string GetExtension(std::string type);
std::string JoinePathToRoot(std::string root, std::string add);
int         CheckHexa(std::string s);
std::string GetRandomName();
