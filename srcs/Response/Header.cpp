#include "Response.hpp"

// Generate timing nedded in Header 
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


// Content length in the Header 
std::string Response::ContentLengthFile(request &r)
{
    std::string Content_Length;
    Content_Length = "Content-Length: ";
    if (r.listDirectory == true)
        Content_Length += my_to_string(r.body.size());
    else
        Content_Length += my_to_string(LengthFile(r.path));
    Content_Length += "\n";
    return Content_Length;
}


// Content type in the header
std::string Response::ContentTypeFile(request &r)
{
    std::string Content_type;
    Content_type = "Content-Type: ";
    if (r.listDirectory == true)
        Content_type += "text/html";
    else
        Content_type += GettingContentType(r.path);
    Content_type += "\n\n";
    return Content_type;
}


// Status Code and Version 
std::string Response::CasesStatusline(request &r)
{
    std::string status_line;
    status_line += r.GeterVersion();
    status_line += " ";
    status_line += my_to_string(r.GetStatutCode());

    switch(r.GetStatutCode()){
        case 200:
            status_line += " OK\n";
            return (status_line);
        case 201:
            status_line += " Created\n";
            return (status_line);
        case 204:
            status_line += " No Content\n";
            return (status_line);
        case 301:
            status_line += " Moved Permanently\n";
            return (status_line);
        default:
            break;
    }
    return("");
}
