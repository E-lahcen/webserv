/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydahni <ydahni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 19:51:32 by ydahni            #+#    #+#             */
/*   Updated: 2023/06/21 01:11:46 by ydahni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <request.hpp>

void    request::ListDirectory()
{
    DIR* d = opendir(this->path.c_str());
    struct dirent* type;
    this->body.clear();
    this->body += "<ul>\n";
    while ((type = readdir(d)) != NULL) {
        this->body += "  <li><a href=";
        this->body += type->d_name;
        this->body += ">";
        this->body += type->d_name;
        this->body += "</a></li>\n";
    }
    this->body += "</ul>\n";
    closedir(d);
}



void request::IsDirectoryGet(iterator_location &itl)
{
    if (this->uri[this->uri.size() -1] == '/')
    {
        if (itl->second.autoindex == 1)
        {
            SetStatutCode(200);
            this->listDirectory = true;
            ListDirectory();
            this->finishRead = 0;
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

void request::Get(iterator_location &itl)
{
    struct stat info;
    if (stat(this->path.c_str(), &info) == 0)
    {
        if (S_ISDIR(info.st_mode) == 1 && this->StatutCode == 0)
        {
            IsDirectoryGet(itl);
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