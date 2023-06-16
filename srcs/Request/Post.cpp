/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrahali <nrahali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 22:02:26 by ydahni            #+#    #+#             */
/*   Updated: 2023/06/15 01:31:38 by nrahali          ###   ########.fr       */
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
        this->path = path;
        std::cout << "path of upload" <<this->path << std::endl;
    }
    else
        this->path = path;
    SetStatutCode(201);
}

void    request::upload(iterator_location &itl, iterator_server &its)
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
        CheckErrorsPage(404, itl, its);      
}



void request::IsDirectoryPost(iterator_location &itl, iterator_server &its)
{
    if (this->uri[this->uri.size() -1] == '/')
    {
        if (itl->second.autoindex == 1)
        {
            std::string index = ScanFolderForIndex(this->path);
            if (index != "NotFound")
            {
                this->path = JoinePathToRoot(this->path , index);
                if (access(this->path.c_str(), R_OK) == 0)
                    IsFile(itl, its);
                else
                    CheckErrorsPage(403, itl, its); 
            }
            else
                CheckErrorsPage(404, itl, its);
        }
        else
            CheckErrorsPage(404, itl, its);
    }
    else
    {
        this->uri += '/';
        SetStatutCode(301);
    }
}

void request::IsFile(iterator_location &itl, iterator_server &its)
{
    std::string search;
    if (this->path.find_first_of(".") != std::string::npos)
        search = this->path.substr(this->path.find_last_of("."));
    if (search == ".py" || search == ".php")
    {
        if (itl->second.cgi.find(search) != itl->second.cgi.end())
        {
            SetStatutCode(200);
            std::cout <<"file path =" <<this->path << std::endl;
            //flag to cgi
            this->cgi = true;
            std::cout << "i have to pass it to cgi" << std::endl;
            this->finishRead = 0;
        }
        else
            CheckErrorsPage(403, itl, its);
    }
    else
    {
        std::cout <<this->path << std::endl;
        SetStatutCode(200);
        this->finishRead = 0;
    }
}

//request Post
void request::Post(iterator_location &itl, iterator_server &its)
{
    if (!itl->second.uploadRoute.empty() && this->StatutCode == 0)
        upload(itl, its);
    else if (itl->second.uploadRoute.empty() && this->StatutCode == 0)
    {
        struct stat info;
        if (stat(this->path.c_str(), &info) == 0)
        {
            if (S_ISDIR(info.st_mode) == 1 && this->StatutCode == 0)
            {
               IsDirectoryPost(itl, its);
            }
            if(S_ISREG(info.st_mode) == 1 && this->StatutCode == 0)
            {
                if (access(this->path.c_str(), R_OK) == 0)
                    IsFile(itl, its);
                else
                    CheckErrorsPage(403, itl, its); 
            }
        }
        else
            CheckErrorsPage(404, itl, its);            
    }
}