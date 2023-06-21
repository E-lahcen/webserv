#include "Response.hpp"


// Header Sending in Case OF Error in CGI (php)

bool Response::HeaderInCaseErrorCGI(request &r)
{
    std::string Header;

    PrintErrors();
    this->Status_Code = 500;
    r.SetStatutCode(500);
    r.path.clear();
    Header =  CasesStatuslineError(r);
    Header += Timing();
    Header += ContentLengthFileError(r);
    Header += ContentTypeFileError();
    
    if (write(fd, Header.c_str() , Header.size()) <= 0)
    {
        this->Closedfile = true;
        this->Error = true;
        return true;
    }
    this->FinishedHeader = true;
    return true;
}

//Header In Case OF CGI exactly .php 
std::string Response::HeaderOFCGI(request &r)
{

    std::string Header;
    size_t size = LengthFile(r.path);
    size_t found;
    char buf[size];

    Header =  CasesStatusline(r);
    Header += Timing();
    size = read(this->OpenedFd, buf, size);
    if (size <= 0)
    {
        this->OpenedFd = -1;
        if (HeaderInCaseErrorCGI(r) == true)
            return ("empty");
    }

    std::string file_php(buf,size);

    if( (found = file_php.find("\r\n\r\n")) != std::string::npos)
    {
        this->HeaderCGI = file_php.substr(0, found);
        file_php.erase(0,found + 4);
    }

    close(this->OpenedFd);

    this->OpenedFd = open(r.path.c_str(), O_RDWR | O_TRUNC , 0777);
    if (this->OpenedFd < 0)
    {
        if (HeaderInCaseErrorCGI(r) == true)
            return ("empty");
    }
        
 
    if (write(this->OpenedFd, file_php.c_str(),file_php.size()) <= 0)
    {
        close(this->OpenedFd);
        this->Closedfile = true;
        this->Error = true;
        return ("empty");
    }

    
    close(this->OpenedFd);


    this->OpenedFd = open(r.path.c_str(), O_RDONLY);
    
    if (this->OpenedFd < 0)
    {
         if (HeaderInCaseErrorCGI(r) == true)
            return ("empty");
    }


    Header += ContentLengthFile(r);
    Header += HeaderCGI;
    Header += "\n\n";
    return (Header);
}
