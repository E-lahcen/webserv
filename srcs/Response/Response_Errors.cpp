#include "Response.hpp"

void Response::PrintHtmlError(std::string errorMessage)
{
    this->error = "<div>\n";
    this->error += "<h1>OOPS!</h1>\n";
    this->error += "<h1>" + errorMessage + "</h1>\n";
    this->error += "</div>\n";
}


//Print Errors
void  Response::PrintErrors()
{
    switch(this->Status_Code){
        case 400:
            PrintHtmlError("400 Bad Request");
            break;
        case 403:
            PrintHtmlError("403 Forbidden");
            break;
        case 404:
            PrintHtmlError("404 Not Found");
            break;
        case 405:
            PrintHtmlError("405 Method Not Allowed");
            break;
        case 409:
            PrintHtmlError("409 Conflict");
            break;
        case 413:
            PrintHtmlError("413 Request Entity Too Large");
            break;
         case 414:
            PrintHtmlError("414 Request-URI Too Long");
            break;
         case 500:
            PrintHtmlError("500 Internal Server Error");
            break;
        case 501:
            PrintHtmlError("501 Not Implemented");
            break;
        case 505:
            PrintHtmlError("505 HTTP Version Not Supported");
        default:
            break;
    }
}



// Status Line in case of error
std::string Response::CasesStatuslineError(request &r)
{
    std::string status_line;
    switch(r.GetStatutCode()){
        case 400:
            status_line += r.GeterVersion();
            status_line += " ";
            status_line += my_to_string(r.GetStatutCode());
            status_line += " Bad Request\n";
            return (status_line);
        case 403:
            status_line += r.GeterVersion();
            status_line += " ";
            status_line += my_to_string(r.GetStatutCode());
            status_line += " Forbidden\n";
            return (status_line);
        case 404:
            status_line += r.GeterVersion();
            status_line += " ";
            status_line += my_to_string(r.GetStatutCode());
            status_line += " Not Found\n";
            return (status_line);
        case 405:
            status_line += r.GeterVersion();
            status_line += " ";
            status_line += my_to_string(r.GetStatutCode());
            status_line += " Method Not Allowed\n";
            return (status_line);
        case 409:
            status_line += r.GeterVersion();
            status_line += " ";
            status_line += my_to_string(r.GetStatutCode());
            status_line += " Conflict\n";
            return (status_line);
        case 413:
            status_line += r.GeterVersion();
            status_line += " ";
            status_line += my_to_string(r.GetStatutCode());
            status_line += " Payload Too Large\n";
            return (status_line);
        case 414:
            status_line += r.GeterVersion();
            status_line += " ";
            status_line += my_to_string(r.GetStatutCode());
            status_line += " URI Too Long\n";
            return (status_line);
        case 500:
            status_line += r.GeterVersion();
            status_line += " ";
            status_line += my_to_string(r.GetStatutCode());
            status_line += " Internal Server Error\n";
            return (status_line);
        case 501:
            status_line += r.GeterVersion();
            status_line += " ";
            status_line += my_to_string(r.GetStatutCode());
            status_line += " Not Implemented\n";
            return (status_line);
        case 505:
            status_line += r.GeterVersion();
            status_line += " ";
            status_line += my_to_string(r.GetStatutCode());
            status_line += " HTTP Version Not Supported\n";
            return (status_line);
        default:
            break;
    }
    return ("");
}


// Content length in case of Error
std::string Response::ContentLengthFileError(request &r)
{
    std::string Content_Length;
    Content_Length = "Content-Length: ";
    switch (r.GetStatutCode())
    {
       case 400:
           Content_Length += my_to_string(LengthFileError(r.path));
           break;
       case 403:
           Content_Length += my_to_string(LengthFileError(r.path));
           break;
       case 404:
           Content_Length += my_to_string(LengthFileError(r.path));
           break;
       case 405:
           Content_Length += my_to_string(LengthFileError(r.path));
           break;
       case 409:
           Content_Length += my_to_string(LengthFileError(r.path));
           break;
       case 413:
           Content_Length += my_to_string(LengthFileError(r.path));
           break;
        case 414:
           Content_Length += my_to_string(LengthFileError(r.path));
           break;
        case 500:
           Content_Length += my_to_string(LengthFileError(r.path));
           break;
        case 501:
           Content_Length += my_to_string(LengthFileError(r.path));
           break;
        case 505:
           Content_Length += my_to_string(LengthFileError(r.path));
           break;
       default:
           break;
    }
    Content_Length += "\n";
    return Content_Length;
}


// Content type (Header) in case of error 
std::string Response::ContentTypeFileError()
{
    std::string Content_type;
    Content_type = "Content-Type: ";
    Content_type += "text/html";;
    Content_type += "\n\n";
    return Content_type;
}


// Headeer Response in case of error
void Response::ResponseHeaderError(request &r)
{
    std::string Header;

    Header =  CasesStatuslineError(r);
    Header += Timing();
    Header += ContentLengthFileError(r);
    Header += ContentTypeFileError();
    
    if (r.GeterMethod() == "POST")
        write(fd, Header.c_str(), Header.size());
    if (r.GeterMethod() == "GET")
        write(fd ,Header.c_str(), Header.size());
    if (r.GeterMethod() == "DELETE")
        write(fd, Header.c_str() , Header.size());
    this->FinishedHeader = true;
}


// Opening File Error For writing Body
void Response::OpeningFileError(std::string path)
{
    if(this->Opening == false)
    {
        std::cout << "open" << std::endl;
        this->Opening = true;
        OpenedFd = open(path.c_str(),O_RDONLY);
        if (OpenedFd < 0)
            PrintErrors();
    }
}

// Writing Body in case of error
void Response::ResponseBodyError()
{
    char buf[1024];
    size_t len = -1;
    if (OpenedFd != -1) 
    {   
        memset(buf, 0, sizeof(buf));
        len = read(OpenedFd, buf, 1024);
        if (len == 0)
        {
            close(OpenedFd);
            this->Closedfile = true;
        }
        if (len > 0)
            write(fd, buf, len);
    }
    else
    {
        write(fd, error.c_str(),error.size());
        this->Closedfile = true;
    }
}


// Error measage in case of error 
void Response::ResponseMessageError(request &r)
{
    OpeningFileError(r.path);
    if (this->FinishedHeader == false)
        ResponseHeaderError(r);
    ResponseBodyError();
}