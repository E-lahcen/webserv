/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydahni <ydahni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 15:20:53 by ydahni            #+#    #+#             */
/*   Updated: 2023/06/06 23:31:13 by ydahni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <request.hpp>

request::request()
{
}

request::~request()
{
}

// parsing all request

void request::GetBody()
{
    if (this->method == "POST" && this->StatutCode == 0)
    {
        if (map.find("Transfer-Encoding") != map.end() && this->StatutCode == 0)
        {
            GetChunked();
        }
        if (map.find("Content-Length") != map.end() && this->StatutCode == 0)
        {
            std::map<std::string, std::string>::iterator it = map.find("Content-Type");
            if (it->second.find("multipart/form-data") != std::string::npos)
            {
                GetMultipart();
            }
            else
            {
                this->body = body.substr(this->body.find("\r\n") + 2, atoi(map["Content-Length"].c_str()));
                if (this->body.length() > this->BodySizeMax)
                    SetStatutCode(413);
            }
        }
    }
}

int request::CheckLocation(std::string location, std::string root)
{
    if (location == this->uri)
    {
        this->path = root;
        return (0);
    }
    std::string a;
    std::string b = this->uri;
    while (location != b)
    {
        size_t n = b.find_last_of("/");
        a = this->uri.substr(n);
        b = b.substr(0, n);
        if (b.empty())
            break;
    }
    if (location == b)
    {
        this->path = JoinePathToRoot(root , a);
        a.clear();
        return (0);
    }
    if (b.empty())
        return (1);
    return (1);
}


// check method if allowed in this server
void request::CheckIfMethodAllowed(std::vector<Server>::iterator its)
{
    std::unordered_map<Path, Server::Location> location = its->getServerLocations();
    std::unordered_map<Path, Server::Location>::iterator itl = location.begin();

    long c = 0;
    for (; itl != location.end(); itl++)
    {
        if (CheckLocation(itl->first, itl->second.root) == 0)
        {
            c++;
            if (itl->second.redirection.first == 301)
            {
                this->path = itl->second.redirection.second;
                std::cout << this->path << std::endl;
                SetStatutCode(301);
                break;
            }
            if (this->method == "POST" && this->StatutCode == 0)
            {
                if (itl->second.post == 1)
                {
                    Post(itl);
                }
                else
                    SetStatutCode(405);
            }
            if (this->method == "GET" && this->StatutCode == 0)
            {
                if (itl->second.get == 1)
                {
                    std::cout << "hi" << std::endl;
                }
                else
                    SetStatutCode(405);
            }
            if (this->method == "DELETE" && this->StatutCode == 0)
            {
                if (itl->second.del == 0)
                    SetStatutCode(405);
            }
            break;
        }
    }
    if (c == 0)
    {
        itl = location.find("/");
        this->path = JoinePathToRoot (itl->second.root , this->uri);
        if (this->method == "POST" && this->StatutCode == 0)
        {
            if (itl->second.post == 1)
            {
                Post(itl);
            }
            else
                SetStatutCode(405);
        }
    }
    location.clear();
}

// start request

void request::GetRequest(Config &config)
{
    Getheader();
    CheckErrors();
    std::vector<Server>::iterator it = config.configServers.begin();
    for (; it != config.configServers.end(); it++)
    {
        if (it->hostname == this->Rhostname && it->port == this->Rport && this->StatutCode == 0)
        {
            this->BodySizeMax = it->getBodySizeMax();
            GetBody();
            if (this->StatutCode == 0)
                CheckIfMethodAllowed(it);
            break;
        }
    }
}
