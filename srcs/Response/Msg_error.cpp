#include "Response.hpp"

// Headeer Response in case of error
void Response::ResponseHeaderError(request &r)
{
    std::string Header;

    Header =  CasesStatuslineError(r);
    Header += Timing();
    Header += ContentLengthFileError(r);
    Header += ContentTypeFileError();
    
    if (r.GeterMethod() == "POST" || r.GeterMethod() == "GET" || r.GeterMethod() == "DELETE")
    {
        if (write(fd, Header.c_str(), Header.size()) <= 0)
        {
            close(OpenedFd);
            this->Closedfile = true;
            this->Error = true;
            return ;
        }
    }  
    this->FinishedHeader = true;
}


// Opening File Error For writing Body
void Response::OpeningFileError(std::string path)
{
    if(this->Opening == false)
    {
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
        len = read(OpenedFd, buf, 1024);
        if (len == 0)
        {
            this->Closedfile = true;
            close(OpenedFd);
        }
        if (len > 0)
        {
            if (write(fd, buf, len) <= 0)
            {
                close(OpenedFd);
                this->Closedfile = true;
                this->Error = true;
                return ;
            }
        }
            
    }
    else
    {
        if(write(fd, error.c_str(),error.size()) <= 0)
        {
            this->Closedfile = true;
            this->Error = true;
            return ;
        }
        this->Closedfile = true;
    }
}


// Error measage in case of error 
void Response::ResponseMessageError(request &r)
{
    OpeningFileError(r.path);
    if (this->FinishedHeader == false)
        ResponseHeaderError(r);
    if (this->Error == true)
        return;
    ResponseBodyError();
}