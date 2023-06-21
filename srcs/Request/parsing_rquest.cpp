/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_rquest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydahni <ydahni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 21:53:31 by ydahni            #+#    #+#             */
/*   Updated: 2023/06/20 20:31:59 by ydahni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <request.hpp>

//parsing header from request

void request::Getheader()
{
    std::vector<std::string> tmp;
    std::stringstream n;
    std::string line = this->header.substr(0, this->header.find("\r\n"));
    std::string line2 = this->header.substr(line.length() + 2);

    n << line;
    while (std::getline(n, line, ' '))
    {
        tmp.push_back(line);
    }
    this->method = tmp[0];
    this->uri = tmp[1];
    this->version = tmp[2];

    if (this->uri.find("?") != std::string::npos)
    {
        this->QueryString = this->uri.substr(this->uri.find("?") + 1);
        this->uri = this->uri.substr(0, this->uri.find("?"));
    }

    tmp.clear();

    n.clear();
    n << line2;
    std::string first;
    std::string secend;

    line.clear();
    while (std::getline(n, line))
    {
        if (line[0] == '\r')
            break;
        first = line.substr(0, line.find(':'));
        secend = line.substr(line.find(':') + 2, line.find("\r") - (first.length() + 2));
        if (first == "Host")
        {
            this->Rhostname = secend.substr(0, secend.find(":"));
            if (this->Rhostname == "localhost")
            {
                this->Rhostname.clear();
                this->Rhostname = "127.0.0.1";
            }
            this->Rport = secend.substr(secend.find(":") + 1);

        }
        this->map.insert(make_pair(first, secend));
        line.clear();
        first.clear();
        secend.clear();
    }
}


size_t convert_hexa(std::string hexa)
{
    size_t decimal;
    std::stringstream h;
    h << std::hex << hexa;
    h >> decimal;
    return (decimal);
}


//parsing request chunked

void request::GetChunked(std::string &body)
{
    std::stringstream n;
    std::string hexa;
    n << body;
    if (this->file != -1)
    {
        getline(n, hexa, '\r');
        getline(n, hexa, '\r');
        if (hexa[0] == '\n')
        {
            if (CheckHexa(hexa) == 1)
            {
                size_t decimal = convert_hexa(hexa);
                if (body.find("\r\n0\r\n") != std::string::npos)
                    this->finishRead = 0;
                if (body.size() >= decimal)
                {
                    size_t p = body.find(hexa);
                    body.erase(p - 1,hexa.length() + 3);
                    if (!body.empty() && decimal != 0)
                    {   
                        this->ContentLength += decimal;
                        if (checkSizeBody() == -1)
                            return ;
                        if (write(this->file, body.substr(0, decimal).c_str(), decimal) <= 0)
                        {
                            ErrorWrite();
                            return ;
                        }
                        body.erase(0, decimal);
                    }
                }
                if (body.find("\r\n0\r\n") != std::string::npos)
                {
                    body.erase(body.find("\r\n0\r\n"));
                    if (!body.empty())
                    {
                        this->ContentLength += body.size();
                        if (checkSizeBody() == -1)
                            return ;
                        if (write(this->file, body.c_str(), body.size()) <= 0)
                        {
                            ErrorWrite();
                            return ;
                        }
                        close(this->file);
                        body.clear();
                    }
                }
            }
        }
    }
}

//for get name of file from  multipart request

std::string GetNameMultipart(std::string header, std::string number, std::string path)
{
    number += "--";
    if (header.find(number) == std::string::npos)
    {
        path += '/';
        std::string name = path;
        if (header.find("filename") != std::string::npos)
        {
            size_t start = header.find("filename") + 10;
            size_t end = header.find("\"", start);
            name += header.substr(start,  end - start);
        }
        else
            name += GetRandomName();
        return (name);
    }
    header.clear();
    return (header);
}

// for parsing multipart request

void request::GetMultipart(std::string &body)
{
    std::string line;
    std::string name;
    std::string number;
    std::string string;


    size_t p =  body.find("\r\n");
    number = body.substr(p + 2, body.find("\r\n", p + 2) - 2);
    size_t i = 0;
    size_t start = 0;
    size_t end = 0;

    p = 0;
    while ((p = body.find(number, i)) != std::string::npos)
    {
        name = body.substr(p, body.find("\r\n\r\n",p) - p);
        if (name.empty())
            break;
        name = GetNameMultipart(name, number, this->path);
        start = body.find("\r\n\r\n", p) + 4;
        end = body.find(number, start);
        string = body.substr(start, end - (start + 2));
        this->Multipart.push_back(std::make_pair(name, string));
        i = p + number.length() + 3;
    }

    std::vector<std::pair<std::string, std::string> >::iterator it = this->Multipart.begin();
    for (; it != Multipart.end(); it++)
    {
        if (it->second.length() > this->BodySizeMax)
        {
            CheckErrorsPage(413);
            return ;
        }
    }

    std::ofstream n;
    it = this->Multipart.begin();
    for (; it != Multipart.end(); it++)
    {
        n.open(it->first);
        n << it->second;
        n.close();
    }
}