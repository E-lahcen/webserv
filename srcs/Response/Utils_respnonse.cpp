#include "Response.hpp"


// Generate timing informationa
std::string Response::Timing()
{
    size_t u = 0;
    std::string DateAndTime;
    std::vector<std::string> line;
    
    time_t currently = time(0);
    std::string DateTime = ctime(&currently);
    while((u = DateTime.find(" ", 0)) != std::string::npos)
    {
        std::string D = DateTime.substr(0, u);
        if (!D.empty())
            line.push_back(D);
        DateTime.erase(0 , u + 1);
    }
    std::string year = DateTime.substr(0, DateTime.find("\n"));
    DateAndTime += "Date: ";
    DateAndTime += line[0];
    DateAndTime += ", ";
    DateAndTime += line[2];
    DateAndTime += " ";
    DateAndTime += line[1];
    DateAndTime += " ";
    DateAndTime += year;
    DateAndTime += " ";
    DateAndTime += line[3];
    DateAndTime += " GMT\n";
    return (DateAndTime);
}


// Convert integer to string
std::string Response::my_to_string(size_t n)
{
    std::stringstream str;
    str << n;
    return (str.str());
}


// calculate Length of File in Case Error
size_t Response::LengthFileError(std::string filename)
{
    FILE *file = fopen(filename.c_str(), "r");
    if (file != nullptr)
    {
        if (fseek(file, 0, SEEK_END) == 0)
            return(ftell(file));
        else
        {
            std::cerr << "Error: fssek Failed" << std::endl;
            return(-1);
        }
    }
    else
    {
        // std::cout << "error = " << this->error << " size " << this->error.size() << std::endl;
        return (this->error.size());
    }
    return(ftell(file));
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
    FILE *file = fopen(filename.c_str(), "r");
    if (file != nullptr)
    {
        if (fseek(file, 0, SEEK_END) == 0)
            return(ftell(file));
        else
        {
            std::cerr << "Error: fssek Failed" << std::endl;
            return(-1);
        }
    }
    else
    {
        // std::cout << "error = " << this->error << " size " << this->error.size() << std::endl;
        std::cerr << " Error Opening file witch calculate length of file " << std::endl;
        return (-1);
    }
    return(ftell(file));
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
         case 414:
            return (true);
         case 500:
            return (true);
         case 501:
            return (true);
        default:
            break;
    }
    return (false);
}