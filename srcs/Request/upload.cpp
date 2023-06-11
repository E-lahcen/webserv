/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upload.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydahni <ydahni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 22:02:26 by ydahni            #+#    #+#             */
/*   Updated: 2023/06/11 15:51:07 by ydahni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <request.hpp>

void request::CreatFileInDirectoryOfUpload(std::string &path)
{
    path += '/';
    path += GetRandomName() + GetExtension(this->map["Content-Type"]);
    std::ofstream file(path.c_str());
    file << this->body;
    file.close();
    SetStatutCode(201);
    path.clear();
}

void    request::upload(std::unordered_map<Path, Server::Location>::iterator &itl)
{
    std::string path = JoinePathToRoot(itl->second.root , itl->second.uploadRoute);
    struct stat info;
    stat(path.c_str(), &info);
    if (S_ISDIR(info.st_mode))
        CreatFileInDirectoryOfUpload(path);
    else
    {
        if (mkdir(path.c_str(), S_IRWXU) == 0)
            CreatFileInDirectoryOfUpload(path);
    }
}

//request Post
void request::Post(std::unordered_map<Path, Server::Location>::iterator &itl)
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
                if (this->uri[this->uri.size() -1] == '/')
                {
                    if (itl->second.autoindex == 1)
                    {
                        std::cout << "yes" << std::endl;
                    }
                    else
                        SetStatutCode(403);
                }
                else
                {
                    this->uri += '/';
                    SetStatutCode(301);
                }
            }
            if(S_ISREG(info.st_mode) == 1 && this->StatutCode == 0)
            {
                if (itl->second.cgi.find(this->path.substr(this->path.find("."))) != itl->second.cgi.end())
                {
                    std::cout << "i have to pass it to cgi" << std::endl;
                }
                else
                    SetStatutCode(403);
            }
        }
        else
            SetStatutCode(404);
    }
    std::cout << GetStatutCode() << std::endl;
}