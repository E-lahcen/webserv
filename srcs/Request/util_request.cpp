/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_request.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydahni <ydahni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 21:50:49 by ydahni            #+#    #+#             */
/*   Updated: 2023/06/21 23:50:26 by ydahni           ###   ########.fr       */
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
    std::string alpha = "aefgklmno6pz1594ruthstuvw8xyuvwbcdjklqrsdsfwxyrstuvwxyz";
    std::vector<std::string> name1;
    std::vector<std::string> name2;
    std::string c;

    int x = 0;
    for (int i = 0; i < 26; i++)
    {
        x = std::rand() % alpha.size();
        c = alpha[x];
        name1.push_back(c);
        x = std::rand() % alpha.size();
        c = alpha[x];
        name2.push_back(c);
    }
    
    int i = std::rand() % name1.size();
    usleep(10);
    // std::srand(std::time(0));
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


void request::Check_methods()
{
    if (this->method == "PUT" || this->method == "PATCH" || this->method == "HEAD" || this->method == "OPTIONS")
        CheckErrorsPage(501);
    else if (this->method == "GET" || this->method == "POST" || this->method == "DELETE")
        SetStatutCode(0);
    else
        CheckErrorsPage(400);
}


//check if header is all good and set statut code

void request::CheckErrorsHeader()
{
    SetStatutCode(0);
    Check_methods();
    if(this->version != "HTTP/1.1")
        CheckErrorsPage(505);
    if(this->map.find("Transfer-Encoding") != this->map.end() && this->StatutCode == 0)
    {
        std::string name;
        name = this->map["Transfer-Encoding"];
        if (name != "chunked")
            CheckErrorsPage(501);
    }
    if (this->method == "POST" && this->StatutCode == 0)
    {
        if (this->map.find("Content-Type") == this->map.end())
            CheckErrorsPage(409);
        if (this->map.find("Transfer-Encoding") == this->map.end() && this->map.find("Content-Length") == this->map.end())
            CheckErrorsPage(400);
        else if (this->map.find("Transfer-Encoding") != this->map.end() && this->map.find("Content-Length") != this->map.end())
            CheckErrorsPage(400);
    }
    if (CheckUri(this->uri) == 0 && this->StatutCode == 0)
        CheckErrorsPage(400);
    if (this->uri.length() > 2048 && this->StatutCode == 0)
        CheckErrorsPage(414);
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
    if (type == "image/svg+xml")
        return (".svg");
    if (type == "image/jpg")
        return (".jpg");
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


//Scan Folder For index file
std::string ScanFolderForIndex(std::string &path)
{
    DIR* d = opendir(path.c_str());
    struct dirent* type;
    while ((type = readdir(d)) != NULL)
    {
        if (type->d_type == DT_REG)
        {
            std::string fileName(type->d_name);
            if (fileName.find("index") != std::string::npos)
            {
                closedir(d);
                return (fileName);
            }
        }
    }
    closedir(d);
    return ("NotFound");
}




void request::CheckErrorsPage(int status_code)
{
    SetStatutCode(status_code);
    this->finishRead = 0;
    if (this->errorPage.find(status_code) != this->errorPage.end())
        this->path = JoinePathToRoot(this->ROT , this->errorPage[status_code]);
    else
        this->path.clear();
}


//check size body
int request::checkSizeBody()
{
    if (this->ContentLength > this->BodySizeMax)
    {
        this->finishRead = 0;
        close(this->file);
        body.clear();
        RemoveFile(this->path);
        CheckErrorsPage(413);
        return (-1);
    }
    return (0);
}

//check Error Write
void request::ErrorWrite()
{
    this->finishRead = 0;
    close(this->file);
    RemoveFile(this->path);
    this->path.clear();
    CheckErrorsPage(500);
    body.clear();
}