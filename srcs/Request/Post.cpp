/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydahni <ydahni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 22:02:26 by ydahni            #+#    #+#             */
/*   Updated: 2023/06/21 21:59:26 by ydahni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <request.hpp>

void request::CreatFileInDirectoryOfUpload(std::string &path)
{
    std::map<std::string, std::string>::iterator it = map.find("Content-Type");
    if (it->second.find("multipart/form-data") == std::string::npos)
    {
        path += '/';
        path += GetRandomName();
        path += GetExtension(this->map["Content-Type"]);
        this->file = open(path.c_str(), O_CREAT | O_RDWR, 0777);
        if (this->file < 0)
        {
            CheckErrorsPage(500);
            return ;
        }
        this->path = path;
    }
    else
        this->path = path;
    SetStatutCode(201);
}

void    request::upload(iterator_location &itl)
{
    struct stat info;
    if (stat(itl->second.root.c_str(), &info) == 0)
    {
        struct stat info;
        std::string path = JoinePathToRoot(itl->second.root , itl->second.uploadRoute);
        stat(path.c_str(), &info);
        if (S_ISDIR(info.st_mode))
            CreatFileInDirectoryOfUpload(path);
        else
        {
            if (mkdir(path.c_str(), S_IRWXU) == 0)
                CreatFileInDirectoryOfUpload(path);
        }
    }
    else
        CheckErrorsPage(404);      
}



void request::IsDirectoryPost(iterator_location &itl)
{
    if (this->uri[this->uri.size() -1] == '/')
    {
        if (itl->second.autoindex == 1)
        {
            for (FilePaths::iterator it = itl->second.defaultFiles.begin(); it != itl->second.defaultFiles.end(); it++)
            {
                this->path = JoinePathToRoot(itl->second.root, *it);
                if (access(this->path.c_str(), F_OK) == 0)
                    break;
            }
            struct stat info;
            if (stat(this->path.c_str(), &info) == 0)
            {
                if (access(this->path.c_str(), R_OK) == 0)
                    IsFile(itl);
                else
                    CheckErrorsPage(403);
            }
            else
                CheckErrorsPage(403);
        }
        else
            CheckErrorsPage(403);
    }
    else
    {
        this->uri += '/';
        this->path = this->uri;
        SetStatutCode(301);
        this->finishRead = 0;
    }
}

void request::CreatFileInDirectoryOfCgi(iterator_location &itl)
{
    this->path = itl->second.root + "/CGIFILE/";
    struct stat info;
    stat(this->path.c_str(), &info);
    if (S_ISDIR(info.st_mode))
    {
        this->path += GetRandomName() + GetExtension(this->map["Content-Type"]);
        this->file = open(this->path.c_str(), O_CREAT | O_RDWR, 0777);
        if (this->file < 0)
        {
            this->cgi = false;
            RemoveFile(this->path);
            CheckErrorsPage(500);
            return ;
        }
    }
    else
    {
        if (mkdir(path.c_str(), S_IRWXU) == 0)
        {
            this->path += GetRandomName() + GetExtension(this->map["Content-Type"]);
            this->file = open(this->path.c_str(), O_CREAT | O_RDWR, 0777);
            if (this->file < 0)
            {
                this->cgi = false;
                RemoveFile(this->path);
                CheckErrorsPage(500);
                return ;
            }
        }
    }
}


void request::IsFile(iterator_location &itl)
{
    std::string search;
    if (this->path.find_first_of(".") != std::string::npos)
        search = this->path.substr(this->path.find_last_of("."));
    if (search == ".py" || search == ".php")
    {
        if (itl->second.cgi.find(search) != itl->second.cgi.end())
        {
            SetStatutCode(200);
            this->cgi = true;
            this->CGIPath = this->path;
            PathExtensionCgi = itl->second.cgi.find(search)->second;
            if (this->method == "POST")
                CreatFileInDirectoryOfCgi(itl);
            if (this->method == "GET")
                this->finishRead = 0;
        }
        else
            CheckErrorsPage(403);
    }
    else
    {
        SetStatutCode(200);
        this->finishRead = 0;
    }
}

//request Post
void request::Post(iterator_location &itl)
{
    if (!itl->second.uploadRoute.empty() && this->StatutCode == 0)
        upload(itl);
    else if (itl->second.uploadRoute.empty() && this->StatutCode == 0)
    {
        struct stat info;
        if (stat(this->path.c_str(), &info) == 0)  
        {
            if (S_ISDIR(info.st_mode) == 1 && this->StatutCode == 0)
            {
               IsDirectoryPost(itl);
            }
            if(S_ISREG(info.st_mode) == 1 && this->StatutCode == 0)
            {
                if (access(this->path.c_str(), R_OK) == 0)
                    IsFile(itl);
                else
                    CheckErrorsPage(403); 
            }
        }
        else
            CheckErrorsPage(404);            
    }
}