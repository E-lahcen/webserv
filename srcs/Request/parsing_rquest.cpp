/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_rquest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydahni <ydahni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 21:53:31 by ydahni            #+#    #+#             */
/*   Updated: 2023/06/04 22:02:01 by ydahni           ###   ########.fr       */
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

    this->header.clear();
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

void request::GetChunked()
{
    std::stringstream b;
    std::ofstream nn;
    std::string line;
    size_t p = 0;

    b << this->body;

    nn.open("image.png");
    while (std::getline(b,line, '\r'))
    {
        if (line[0] == '\n')
        {
            if (CheckHexa(line) == 1)
            {
                p = this->body.find(line);
                long decimal = strtol(line.substr(1).c_str(), NULL, 16);
                if (decimal == 0)
                    break;
                this->body.erase(p - 1,line.length() + 3);
            }
        }
    }
    p = this->body.find("\r\n0\r\n");
    this->body.erase(p);
    nn << this->body;
    if ((this->body.length()) > this->BodySizeMax)
        SetStatutCode(413);
}

//for get name of file from  multipart request

std::string GetNameMultipart(std::string header, std::string number)
{
    number += "--";
    if (header.find(number) == std::string::npos)
    {
        std::string name;
        if (header.find("filename") != std::string::npos)
        {
            size_t start = header.find("filename") + 10;
            size_t end = header.find("\"", start);
            name = header.substr(start,  end - start);
        }
        else
            name = GetRandomName();
        return (name);
    }
    header.clear();
    return (header);
}

// for parsing multipart request

void request::GetMultipart()
{
    std::string line;
    std::string name;
    std::string number;
    std::string body;


    size_t p =  this->body.find("\r\n");
    number = this->body.substr(p + 2, this->body.find("\r\n", p + 2) - 2);

    size_t i = 0;
    size_t start = 0;
    size_t end = 0;

    p = 0;
    while ((p = this->body.find(number, i)) != std::string::npos)
    {
        name = this->body.substr(p, this->body.find("\r\n\r\n",p) - p);
        if (name.empty())
            break;
        name = GetNameMultipart(name, number);
        start = this->body.find("\r\n\r\n", p) + 4;
        end = this->body.find(number, start);
        body = this->body.substr(start, end - (start + 2));
        this->Multipart.push_back(std::make_pair(name, body));
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