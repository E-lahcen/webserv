/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 15:20:13 by ydahni            #+#    #+#             */
/*   Updated: 2023/06/07 17:20:48 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef request_hpp
#define request_hpp

#include <Config.hpp>

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
        std::string header;
        //body
        std::string body;
        std::vector<std::pair<std::string, std::string> > Multipart;
        //
        size_t BodySizeMax;
        std::string Rhostname;
	    std::string Rport;
    
        //for respons
        std::string path;
        int StatutCode;
    public:
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
        void GetRequest(Config  &config);
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
int CheckHexa(std::string s);
std::string GetRandomName();

#endif