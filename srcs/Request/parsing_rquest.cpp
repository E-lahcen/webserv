/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_rquest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydahni <ydahni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 21:53:31 by ydahni            #+#    #+#             */
/*   Updated: 2023/06/13 21:54:02 by ydahni           ###   ########.fr       */
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

    // std::map<std::string, std::string>::iterator it = map.begin();
    // for(; it != map.end(); it++)
    //     std::cout << it->first << " " << it->second << std::endl;
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
                size_t decimal = strtol(hexa.c_str(), NULL, 16);
                if (body.find("\r\n0\r\n") != std::string::npos)
                    this->finishRead = 0;
                if (body.size() >= decimal)
                {
                    size_t p = body.find(hexa);
                    body.erase(p - 1,hexa.length() + 3);
                    write(this->file, body.substr(0, decimal).c_str(), decimal);
                    body.erase(0, decimal);
                }
                if (body.find("\r\n0\r\n") != std::string::npos)
                {
                    body.erase(body.find("\r\n0\r\n"));
                    write(this->file, body.c_str(), body.size());
                    body.clear();
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
    std::ofstream n;
    for (; it != Multipart.end(); it++)
    {
        n.open(it->first);
        n << it->second;
        if (it->second.length() > this->BodySizeMax)
            SetStatutCode(413);
        n.close();
    }
}