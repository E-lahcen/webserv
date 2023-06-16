/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrahali <nrahali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 15:20:13 by ydahni            #+#    #+#             */
/*   Updated: 2023/06/15 01:31:15 by nrahali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Config.hpp>
#include <Network.hpp>


typedef std::vector<Server> Servers;
class Config;

//check if directory
#include <sys/stat.h>

//Struct for Scan Folder
#include <dirent.h>

//for request 
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <unistd.h>

typedef std::unordered_map<Path, Server::Location>::iterator iterator_location;
typedef std::vector<Server>::iterator iterator_server;
class request
{
    private:
        //header
        std::string method;
        std::string uri;
        std::string version;
        //body
        int file;
        std::vector<std::pair<std::string, std::string> > Multipart;
        //
        size_t BodySizeMax;
        std::string Rhostname;
	    std::string Rport;
    
        //for respons
        int StatutCode;
    public:
        std::map<std::string, std::string> map;
        //for check response
        bool cgi;
        bool listDirectory;
        std::string path;

        int finishRead;
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

        void Check_methods(iterator_location &itl, iterator_server &its);

        //for request resources
        int my_main();
        void GetRequest( Servers& servers );
        void GetBody(std::string &body);
        void Getheader();

        //Content-Length
        void GetContentLength(std::string &body);
        size_t ContentLength;

        //chunked
        void GetChunked(std::string &body);

        //multipart
        int multipart;
        void GetMultipart(std::string &body);

        //
        int CheckLocation(std::string location, std::string root);
        void CheckIfMethodAllowed(iterator_server &its);
        //Post
        void Post(iterator_location &itl, iterator_server &its);
        void upload(iterator_location &itl,iterator_server &its);
        void CreatFileInDirectoryOfUpload(std::string &path);
        void IsDirectoryPost(iterator_location &itl, iterator_server &its);
        void IsFile(iterator_location &itl, iterator_server &its);

        //Get
        void Get(iterator_location &itl, iterator_server &its);
        void IsDirectoryGet(iterator_location &itl, iterator_server &its);
        void ListDirectory();

        //for error
        void CheckErrorsHeader(iterator_location &itl, iterator_server &its);

        //check errors page
        void CheckErrorsPage(int status_code, iterator_location &itl, iterator_server &its);
        void handleMethodLocation(iterator_location &itl, iterator_server &its);
        
        //for class
        request();
        ~request();
};


std::string GetExtension(std::string type);
std::string JoinePathToRoot(std::string root, std::string add);
int CheckHexa(std::string s);
std::string GetRandomName();
std::string ScanFolderForIndex(std::string &path);
