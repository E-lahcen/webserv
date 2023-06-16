/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydahni <ydahni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 15:20:53 by ydahni            #+#    #+#             */
/*   Updated: 2023/06/15 00:08:04 by ydahni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <request.hpp>

request::request()
{
    this->file = -1;
    this->multipart = -1;
    this->ContentLength = 0;
    this->finishRead = -1;
    this->cgi = false;
    this->listDirectory = false;
    this->StatutCode = 0;
    
}

request::~request()
{
}
//Content-Length

void request::GetContentLength(std::string &body)
{
    std::stringstream number;
    number << map["Content-Length"].c_str();
    size_t size = 0;
    number >> size;
    if (this->file != -1)
    {
        if (this->ContentLength == 0)
        {
            body = body.substr(this->body.find("\r\n") + 3);
        }
        this->ContentLength += body.size();
        if (this->ContentLength == size)
        {
            if(!body.empty())
            {
                write(this->file, body.c_str(), body.size());
                body.clear();
            }
            if (this->ContentLength > this->BodySizeMax)
                SetStatutCode(413);
            this->finishRead = 0;
            return;
        }
        else
        {
            write(this->file, body.c_str(), body.size());
            body.clear();
        }
    }
}
// parsing all request

void request::GetBody(std::string &body)
{
    if (this->method == "POST" )
    {
        if (map.find("Transfer-Encoding") != map.end())
        {
            GetChunked(body);
        }
        if (map.find("Content-Length") != map.end())
        {
            std::map<std::string, std::string>::iterator it = map.find("Content-Type");
            if (it->second.find("multipart/form-data") != std::string::npos)
            {
                std::stringstream number;
                number << map["Content-Length"].c_str();
                size_t size = 0;
                number >> size;
                if (body.size() == (size + 2))
                    this->multipart = 1;
            }
            else
            {
                GetContentLength(body);
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


void request::handleMethodLocation(std::unordered_map<Path, Server::Location>::iterator &itl, std::vector<Server>::iterator &its)
{
    if (this->method == "POST" && this->StatutCode == 0)
    {
        if (itl->second.post == 1)
        {
            Post(itl, its);
        }
        else
            CheckErrorsPage(405, itl, its);
    }
    if (this->method == "GET" && this->StatutCode == 0)
    {
        if (itl->second.get == 1)
        {
            Get(itl, its);
        }
        else
            CheckErrorsPage(405, itl, its);
    }
    if (this->method == "DELETE" && this->StatutCode == 0)
    {
        if (itl->second.del == 0)
            CheckErrorsPage(405, itl, its);
    }
}



// check method if allowed in this server
void request::CheckIfMethodAllowed(iterator_server &its)
{
    std::unordered_map<Path, Server::Location> location = its->getServerLocations();
    std::unordered_map<Path, Server::Location>::iterator itl = location.begin();

    long c = 0;
    for (; itl != location.end(); itl++)
    {
        if (CheckLocation(itl->first, itl->second.root) == 0)
        {
            c++;
            CheckErrorsHeader(itl, its);
            if (itl->second.redirection.first == 301)
            {
                this->path = itl->second.redirection.second;
                std::cout << this->path << std::endl;
                this->finishRead = 0;
                SetStatutCode(301);
                break; 
            }
            if (itl->first == this->uri)
            {
                if (!itl->second.defaultFile.empty())
                    this->path = JoinePathToRoot(itl->second.root, itl->second.defaultFile);
                std::cout << this->path << std::endl;
            }
            handleMethodLocation(itl, its);
            break;
        }
    }
    if (c == 0)
    {
        itl = location.find("/");
        this->path = JoinePathToRoot (itl->second.root , this->uri);
        CheckErrorsHeader(itl, its);
        if (itl->second.redirection.first == 301)
        {
            this->path = itl->second.redirection.second;
            std::cout << this->path << std::endl;
            this->finishRead = 0;
            SetStatutCode(301);
            return ;
        }
        if (itl->first == this->uri)
        {
            if (!itl->second.defaultFile.empty())
                this->path = JoinePathToRoot(itl->second.root, itl->second.defaultFile);
        }
        handleMethodLocation(itl, its);
    }
    location.clear();
}

// start request

void request::GetRequest( Servers& servers )
{
    Getheader();
    std::vector<Server>::iterator it = servers.begin();
    for (; it != servers.end(); it++)
    {
        if (it->hostname == this->Rhostname && it->port == this->Rport && this->StatutCode == 0)
        {
            this->BodySizeMax = it->getBodySizeMax();
            if (this->StatutCode == 0)
                CheckIfMethodAllowed(it);
            break;
        }
    }
}
