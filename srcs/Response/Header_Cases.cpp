#include "Response.hpp"

// Header and body in case of Created Data POST 
void Response::HeaderAndBodyOfCreated(request &r)
{
    std::string line;

    line = CasesStatusline(r);
    line += "Location: ";
    line += r.path;
    line += "\nContent-Type: text/plain\n\n";
    line += "The resource has been successfully created.";
    if(write(fd, line.c_str(), line.size()) <= 0)
    {
        this->Closedfile = true;
        this->Error = true;
        return ;
    }
}


// Rederection Header Response
void Response::HeaderOfRedirect(request &r)
{
    std::string Header;

    Header = CasesStatusline(r);
    Header += "Location: ";
    Header += r.path;
    if (write(fd, Header.c_str(), Header.size()) <= 0)
    {
        this->Closedfile = true;
        this->Error = true;
        return ;
    } 
}

// Delete Header Response
void Response::HeaderOfDelete(request &r)
{
    std::string Header;

    Header = CasesStatusline(r);
    Header += Timing();
    Header += "Content-Length: 0\n";
    if (write(fd, Header.c_str(), Header.size()) <= 0)
    {
        this->Closedfile = true;
        this->Error = true;
        return ;
    } 
}
