#include "Response.hpp"

Response::Response()
{
      this->Closedfile = false;
      this->FinishedHeader = false;
      this->Opening = false;
      this->OpenedFd = -1;
}

// Header and body of case of Created Data POST 
void Response::HeaderAndBodyOfCreated(request &r)
{
    std::string line;

    line = CasesStatusline(r);
    line += "Location: ";
    line += r.path;
    line += "\nContent-Type: text/plain\n\n";
    line += "The resource has been successfully created.";
    write(fd, line.c_str(), line.size());
}


// Response Message Handler
void Response::ResponseMessage(request &r)
{
    this->Status_Code = r.GetStatutCode();
    // std::cout << "path is = " << r.path << std::endl;
    if (this->Status_Code == 301)
    {
        std::cout << "path is = " << r.path << std::endl;
        HeaderOfRedirect(r);
        this->Closedfile = true;
        return ;
    }
    if (this->Status_Code == 201)
    {
        HeaderAndBodyOfCreated(r);
        this->Closedfile = true;
        return ;
    }
    if (CheckingStatusCode(Status_Code) == true)
        this->ResponseMessageError(r);
    else
    {
        OpeningFile(r.path);
        if (this->FinishedHeader == false)
            ResponseHeader(r);
        ResponseBody(r);
    }
}
