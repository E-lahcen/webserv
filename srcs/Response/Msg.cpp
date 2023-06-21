#include "Response.hpp"


// Header Sending in case of Error opning file 
bool Response::HeaderInCaseError(request &r)
{
    std::string Header;

    this->Status_Code = 500;
    r.SetStatutCode(500);
    r.path.clear();
    Header =  CasesStatuslineError(r);
    Header += Timing();
    Header += ContentLengthFileError(r);
    Header += ContentTypeFileError();
    if (write(fd, Header.c_str() , Header.size()) <= 0)
    {
        close(OpenedFd);
        this->Closedfile = true;
        this->Error = true;
        return true;
    }
    this->FinishedHeader = true;
    return true;
}



// Headeer Response
void Response::ResponseHeader(request &r)
{
    if (this->OpenedFd < 0)
    {
        if (HeaderInCaseError(r) == true)
            return ;
    }

    std::string Header;

    Header =  CasesStatusline(r);
    Header += Timing();
    Header += ContentLengthFile(r);
    Header += ContentTypeFile(r);

    

    if (!storpath.empty())
    {
        r.RemoveFile(storpath);
        storpath.clear();
    }

    if(GettingExtension(r.CGIPath) == ".php")
    {
        storpath = r.path;
        Header.clear();
        Header = HeaderOFCGI(r);
        if (Header == "empty")
            return ;
    }

    if (r.GeterMethod() == "POST" || r.GeterMethod() == "GET" || r.GeterMethod() == "DELETE")
    {
        if (write(fd, Header.c_str() , Header.size()) <= 0)
        {
            close(OpenedFd);
            this->Closedfile = true;
            this->Error = true;
            return ;
        }
    }
    this->FinishedHeader = true;
}




// Opening File For writing Body 
void  Response::OpeningFile(std::string path)
{
    if(this->Opening == false)
    {
        this->Opening = true;
        OpenedFd = open(path.c_str(),O_RDONLY);
        if (OpenedFd < 0)
            PrintErrors();
    }
}



// Writing Body in normal case
void Response::ResponseBody(request &r)
{
    char buf[1024];
    size_t len = -1;
    if (r.listDirectory == true)
    {
        if(write(fd, r.body.c_str(), r.body.size()) <= 0)
        {
            this->Closedfile = true;
            close(OpenedFd);
            return ;
        }
        this->Closedfile = true;
        close(OpenedFd);
        return ;
    }
    if (OpenedFd != -1)
    {
        len = read(OpenedFd, buf, 1024);
        if (len <= 0)
        {
            if (!storpath.empty())
            {
                r.RemoveFile(storpath);
                storpath.clear();
            }
            this->Closedfile = true;
            close(OpenedFd);
        }
        if (len > 0)
        {
            if (write(fd, buf, len) <= 0)
            {
                this->Closedfile = true;
                close(OpenedFd);
                return ;
            }
        }
           
    }
    else
    {
        if(write(fd, error.c_str(),error.size()) <= 0)
        {
            this->Closedfile = true;
            return ;
        }
        this->Closedfile = true;
    }
}




