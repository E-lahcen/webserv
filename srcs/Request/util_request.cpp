/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_request.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydahni <ydahni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 21:50:49 by ydahni            #+#    #+#             */
/*   Updated: 2023/06/06 22:02:53 by ydahni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <request.hpp>

void request::printHeader() const
{
    std::cout << std::endl <<  this->header << std::endl;
}

// for check if hexa exist in chunked
int CheckHexa(std::string s)
{
    if (s.size() > 7)
        return (0);
    s = s.substr(1);
    if (s.empty())
        return 0;
    int i = 0;
    while (s[i])
    {
        if (!isxdigit(s[i]))
            return (0);
        i++;
    }
    return (1);
}


//generate a random name for file in multipart

std::string GetRandomName()
{
    std::string alpha = "abcdefghijklmnopqrstuvwxyz";
    std::vector<std::string> name1;
    std::vector<std::string> name2;
    std::string c;

    for (int i = 0; i < 26; i++)
    {
        c = alpha[i];
        name1.push_back(c);
    }

    for (int i = 26 ; i > 0; i--)
    {
        c = alpha[i];
        name2.push_back(c);
    }
    
    std::srand(std::time(0));
    int i = std::rand() % name1.size();
    sleep(1);
    std::srand(std::time(0));
    int j = std::rand() % name2.size();

    std::string name = name1[i] + name2[j];

    alpha.clear();
    c.clear();
    name1.clear();
    name2.clear();

    return (name);
}

//check uri if valid

int CheckUri(std::string uri)
{
    std::string allowedchar = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
    int i = 0;
    while (uri[i])
    {
        if (uri[i] == '%')
        {
            if(isxdigit(uri[i + 1]) && isxdigit(uri[i + 2]))
                return (0);
        }
        if (allowedchar.find(uri[i]) == std::string::npos)
            return (0);
        i++;
    }
    return (1);
}

//check if header is all good and set statut code

void request::CheckErrors()
{
    SetStatutCode(0);

    if(this->map.find("Transfer-Encoding") != this->map.end() && this->StatutCode == 0)
    {
        std::string name;
        name = this->map["Transfer-Encoding"];
        if (name != "chunked")
            SetStatutCode(501);
    }
    if (this->method == "POST" && this->StatutCode == 0)
    {
        if (this->map.find("Transfer-Encoding") == this->map.end() && this->map.find("Content-Length") == this->map.end())
            SetStatutCode(400);
    }
    if (CheckUri(this->uri) == 0 && this->StatutCode == 0)
        SetStatutCode(400);
    if (this->uri.length() > 2048 && this->StatutCode == 0)
        SetStatutCode(414);
}

//join to root
std::string JoinePathToRoot(std::string root, std::string add)
{
    if (add[0] == '/' && root[root.size() - 1] != '/')
    {
        return (root + add);
    }
    if (add[0] != '/' && root[root.size() - 1] == '/')
    {
        return (root + add);
    }
    if (add[0] == '/' && root[root.size() - 1] == '/')
    {
        root = root.substr(0, root.size() - 1);
        return (root + add);
    }
    if (add[0] != '/' && root[root.size() - 1] != '/')
    {
        root += '/';
        return (root + add);
    }
    return (root + add);
}


//for get extention
std::string GetExtension(std::string type)
{
    if (type == "text/plain")
        return (".txt");
    if (type == "text/html")
        return (".html");
    if (type == "text/css")
        return (".css");
    if (type == "text/javascript")
        return (".js");
    if (type == "application/json")
        return (".json");
    if (type == "application/x-www-form-urlencoded")
        return ("");
    if (type == "multipart/form-data")
        return ("");
    if (type == "image/jpeg")
        return (".jpeg");
    if (type == "image/png")
        return (".png");
    if (type == "image/gif")
        return (".gif");
    if (type == "audio/mpeg")
        return (".mp3");
    if (type == "video/mp4")
        return (".mp4");
    if (type == "application/pdf")
        return (".pdf");
    return ("");
}