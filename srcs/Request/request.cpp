/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 15:20:53 by ydahni            #+#    #+#             */
/*   Updated: 2023/06/21 03:14:42 by lelhlami         ###   ########.fr       */
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



void request::MultipartForCGI(std::string &body)
{
    std::stringstream number;
    number << map["Content-Length"].c_str();
    size_t size = 0;
    number >> size;

    if (size > this->BodySizeMax)
    {
        this->ContentLength = size;
        this->cgi = false;
        checkSizeBody();
        return ;
    }
    if (this->file != -1)
    {
        if (this->ContentLength == 0)
        {
            body = body.substr(this->body.find("\r\n") + 3);
            this->ContentLength = 3;
        }
        this->ContentLength += body.size();
        if (this->ContentLength >= size)
        {
            if(!body.empty())
            {
                if (write(this->file, body.c_str(), body.size()) <= 0)
                {
                    this->cgi = false;
                    ErrorWrite();
                    return ;
                }
                close(this->file);
                body.clear();
            }
            this->finishRead = 0;
        }
        else
        {
            if (!body.empty())
            {
                if (write(this->file, body.c_str(), body.size()) <= 0)
                {
                    this->cgi = false;
                    ErrorWrite();
                    return ;
                }
                body.clear();
            }
        }
    }
}


//Content-Length

void request::GetContentLength(std::string &body)
{
    std::stringstream number;
    number << map["Content-Length"].c_str();
    size_t size = 0;
    number >> size;

    if (size > this->BodySizeMax)
    {
        this->ContentLength = size;
        this->cgi = false;
        checkSizeBody();
        return ;
    }

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
                if (write(this->file, body.c_str(), body.size()) <= 0)
                {
                    this->cgi = false;
                    ErrorWrite();
                    return ;
                }
                close(this->file);
                body.clear();
            }
            this->finishRead = 0;
        }
        else
        {
            if (!body.empty())
            {
                if (write(this->file, body.c_str(), body.size()) <= 0)
                {
                    this->cgi = false;
                    ErrorWrite();
                    return ;
                }
                body.clear();
            }
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
                if (this->cgi == true)
                {
                    MultipartForCGI(body);
                }
                if (this->cgi == false)
                {
                    std::stringstream number;
                    number << map["Content-Length"].c_str();
                    size_t size = 0;
                    number >> size;
                    if (body.size() == (size + 2))
                        this->multipart = 1;
                }
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


void request::handleMethodLocation(iterator_location &itl)
{
    if (this->method == "POST" && this->StatutCode == 0)
    {
        if (itl->second.post == 1)
        {
            Post(itl);
        }
        else
            CheckErrorsPage(405);
    }
    if (this->method == "GET" && this->StatutCode == 0)
    {
        if (itl->second.get == 1)
        {
            Get(itl);
        }
        else
            CheckErrorsPage(405);
    }
    if (this->method == "DELETE" && this->StatutCode == 0)
    {
        if (itl->second.del == 1)
        {
            Delete();
        }
        else
            CheckErrorsPage(405);
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
            this->ROT = itl->second.root;
            CheckErrorsHeader();
            if (GetStatutCode() == 0)
            {
                if (itl->second.redirection.first == 301)
                {
                    this->path = itl->second.redirection.second;
                    this->finishRead = 0;
                    SetStatutCode(301);
                    break; 
                }
                if (itl->first == this->uri)
                {
                    for (FilePaths::iterator it = itl->second.defaultFiles.begin(); it != itl->second.defaultFiles.end(); it++)
                    {
                        this->path = JoinePathToRoot(itl->second.root, *it);
                        if (access(this->path.c_str(), F_OK) == 0)
                            break;
                    }
                    struct stat info;
                    if (stat(this->path.c_str(), &info) != 0)
                    {
                        CheckErrorsPage(404);
                        return ;
                    }
                }
                handleMethodLocation(itl);
                break;
            }
            else
                break;
        }
    }
    if (c == 0)
    {
        itl = location.find("/");
        this->ROT = itl->second.root;
        this->path = JoinePathToRoot (itl->second.root , this->uri);
        CheckErrorsHeader();
        if (GetStatutCode() == 0)
        {
            if (itl->second.redirection.first == 301)
            {
                this->path = itl->second.redirection.second;
                this->finishRead = 0;
                SetStatutCode(301);
                return ;
            }
            if (itl->first == this->uri)
            {
                for (FilePaths::iterator it = itl->second.defaultFiles.begin(); it != itl->second.defaultFiles.end(); it++)
                {
                    this->path = JoinePathToRoot(itl->second.root, *it);
                    if (access(this->path.c_str(), F_OK) == 0)
                        break;
                }
                struct stat info;
                if (stat(this->path.c_str(), &info) != 0)
                {
                    CheckErrorsPage(404);
                    return ;
                }
            }
            handleMethodLocation(itl);
        }
    }
    location.clear();
}

// start request

void request::GetRequest( Servers& servers )
{
    Getheader();
    std::vector<Server>::iterator its = servers.begin();
    for (; its != servers.end(); its++)
    {
        if (its->hostname == this->Rhostname && its->port == this->Rport && this->StatutCode == 0)
        {
            this->errorPage =  its->errorPage;
            this->BodySizeMax = its->getBodySizeMax();
            if (this->StatutCode == 0)
                CheckIfMethodAllowed(its);
            break;
        }
    }
}
