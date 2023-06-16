#include <Response.hpp>
#include <utils.hpp>



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



// Headeer Response
void Response::ResponseHeader(request &r)
{
    std::string Header;

    Header =  CasesStatusline(r);
    Header += Timing();
    Header += ContentLengthFile(r);
    Header += ContentTypeFile(r);

    if (r.GeterMethod() == "POST")
        write(fd, Header.c_str(), Header.size());
    if (r.GeterMethod() == "GET")  
        write(fd ,Header.c_str(), Header.size());
    if (r.GeterMethod() == "DELETE")
        write(fd, Header.c_str() , Header.size());
    this->FinishedHeader = true;
}


// Opening File For writing Body 
void Response::OpeningFile(std::string path)
{
    if(this->Opening == false)
    {
        this->Opening = true;
        OpenedFd = open(path.c_str(),O_RDONLY);
    }
}


// Writing Body in case of error
void Response::ResponseBody(request &r)
{
    char buf[1024];
    size_t len = -1;

    if (r.listDirectory == true)
    {
        write(fd, r.body.c_str(), r.body.size());
        this->Closedfile = true;
        return ;
    }
    if (OpenedFd != -1) 
    {
        memset(buf, 0, sizeof(buf));
        len = read(OpenedFd, buf, 1024);
        if (len == 0)
        {
            this->Closedfile = true;
            close(OpenedFd);
        }
        if (len > 0)
            write(fd, buf, len);
    }
    else
    {
        std::cerr << "Error of opning File in normal Case" << std::endl;
        return ;
    }
}


// Rederection Header response
void Response::HeaderOfRedirect(request &r)
{
    std::string Header;

    Header = CasesStatusline(r);
    Header += "Location: ";
    Header += r.path;
    write(fd, Header.c_str(), Header.size());
}



// Status Code and Version 
std::string Response::CasesStatusline(request &r)
{
    std::string status_line;
    switch(r.GetStatutCode()){
        case 200:        
            status_line += r.GeterVersion();
            status_line += " ";
            status_line += my_to_string(r.GetStatutCode());
            status_line += " OK\n";
            return (status_line);
        case 201:
            status_line += r.GeterVersion();
            status_line += " ";
            status_line += my_to_string(r.GetStatutCode());
            status_line += " Created\n";
            return (status_line);
        case 204:
            status_line += r.GeterVersion();
            status_line += " ";
            status_line += my_to_string(r.GetStatutCode());
            status_line += " No Content\n";
            return (status_line);
        case 301:
            status_line += r.GeterVersion();
            status_line += " ";
            status_line += my_to_string(r.GetStatutCode());
            status_line += " Moved Permanently\n";
            return (status_line);
        default:
            break;
    }
    return("");
}
