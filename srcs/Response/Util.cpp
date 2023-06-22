#include "Response.hpp"

// Convert integer to string
std::string Response::my_to_string(size_t n)
{
    std::stringstream str;
    str << n;
    return (str.str());
}


//Getting The Extension For path handling the CGI
std::string Response::GettingExtension(std::string path)
{
    if (path.find_last_of(".") != std::string::npos)
        return (path.substr(path.find_last_of(".")));
    return ("");
}



//Getting  The Content type through the extention
std::string Response::GettingContentType(std::string path)
{
    if (path.find_last_of(".") != std::string::npos)
        return (FindingExten(path.substr(path.find_last_of("."))));
    return ("");
}


//Finding Extension 
std::string Response::FindingExten(std::string extension)
{
    if (extension == ".txt")
        return ("text/plain");
    if (extension == ".html")
        return ("text/html");
    if (extension == ".css")
        return ("text/css");
    if (extension == ".js")
        return ("text/javascript");
    if (extension == ".json")
        return ("application/json");
    if (extension == ".jpg")
        return ("image/jpg");
    if (extension == ".svg")
        return ("image/svg+xml");
    if (extension == ".jpeg")
        return ("image/jpeg");
    if (extension == ".png")
        return ("image/png");
    if (extension == ".gif")
        return ("image/gif");
    if (extension == ".mp3")
        return ("audio/mpeg");
    if (extension == ".mp4")
        return ("video/mp4");
    if (extension == ".pdf")
        return ("application/pdf"); 
    return ("");
}


// calculate Length of File 
size_t Response::LengthFile(std::string filename)
{
    size_t size = 0;
    FILE *file = fopen(filename.c_str(), "r");
    if (file != nullptr)
    {
        if (fseek(file, 0, SEEK_END) == 0)
        {
            size = ftell(file);
            fclose(file);
            return(size);
        }
    }
    else
    {
        fclose(file);
        return (0);
    }
    return(0);
}



//Checking if the status code is a error 
bool Response::CheckingStatusCode(int status_code)
{
    switch(status_code)
    {
        case 400:
            return (true);
        case 403:
            return (true);
        case 404:
            return (true);
        case 405:
            return (true);
        case 409:
            return (true);
        case 413:
            return (true);
        case 500:
           return (true);
        case 501:
           return (true);
        case  505:
           return (true);
        default:
            break;
    }
    return (false);
}


