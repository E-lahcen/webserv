/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydahni <ydahni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 15:20:13 by ydahni            #+#    #+#             */
/*   Updated: 2023/06/20 22:11:19 by ydahni           ###   ########.fr       */
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
#include <cstdio>

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
        size_t BodySizeMax;
        std::vector<std::pair<std::string, std::string> > Multipart;
    
        //for respons
        int StatutCode;
    public:
        std::unordered_map<short, std::string> errorPage;
        std::string ROT;
        //for response
        bool cgi;
        bool listDirectory;
        int finishRead;
        std::string PathExtensionCgi;
        std::string path;
        std::string CGIPath;
        std::string QueryString;
	    std::string Rport;
        std::string Rhostname;
        std::string header;
        std::string body;
        std::map<std::string, std::string> map;


        //for request resources
        int my_main();
      

        //util request
        void printHeader() const;
        void Check_methods();
        void CheckErrorsHeader();
        void CheckErrorsPage(int status_code);
        int checkSizeBody();
        void ErrorWrite();

    


        //request
        void MultipartForCGI(std::string &body);
        void GetContentLength(std::string &body);
        size_t ContentLength;
        int CheckLocation(std::string location, std::string root);
        void handleMethodLocation(iterator_location &itl);
        void CheckIfMethodAllowed(iterator_server &its);
        void GetRequest( Servers& servers );
        



        //parsing request
        void Getheader();
        void GetChunked(std::string &body);
        void GetMultipart(std::string &body);
        int multipart;
        void GetBody(std::string &body);
        


        //Geters
        void SetStatutCode(int statut);
        int GetStatutCode() const;
        std::string GeterMethod() const;
        std::string GeterUri() const;
        std::string GeterVersion() const;
        std::map<std::string, std::string> GeterHeader() const;
        
        
        //Post
        void CreatFileInDirectoryOfUpload(std::string &path);
        void upload(iterator_location &itl);
        void IsDirectoryPost(iterator_location &itl);
        void CreatFileInDirectoryOfCgi(iterator_location &itl);
        void IsFile(iterator_location &itl);
        void Post(iterator_location &itl);


        //Get
        void ListDirectory();
        void IsDirectoryGet(iterator_location &itl);
        void Get(iterator_location &itl);


        //Delete
        int RemoveDirectory(std::string pah);
        int RemoveFile(std::string path);
        void Delete();



        //for class
        request();
        ~request();
};


//util request
int CheckHexa(std::string s);
std::string GetExtension(std::string type);
std::string JoinePathToRoot(std::string root, std::string add);
std::string GetRandomName();
std::string ScanFolderForIndex(std::string &path);
