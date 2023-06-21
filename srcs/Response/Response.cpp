#include "Response.hpp"

Response::Response()
{
      this->Closedfile = false;
      this->FinishedHeader = false;
      this->Opening = false;
      this->OpenedFd = -1;
      this->child_pid = -1;
      this->wait_child = -1;
      this->OPennedFdCGI = -1;
      this->wait = false;
      this->Error = false;
}


// Response Message Handler
void Response::ResponseMessage(request &r)
{
    this->Status_Code = r.GetStatutCode();
    if (r.cgi == true)
    {
        r.cgi = false;
        this->wait = true;
        CGI(r);
    }
    if (CheckChildPid() != 0 || (CheckChildPid() == 0  && this->wait == false))
    { 
        if (this->Status_Code == 204 && this->Error == false)
        {
            HeaderOfDelete(r);
            this->Closedfile = true;
            return ;
        }
        if (this->Status_Code == 301 && this->Error == false)
        {
            HeaderOfRedirect(r);
            this->Closedfile = true;
            return ;
        }

        if (this->Status_Code == 201 && this->Error == false)
        {
            HeaderAndBodyOfCreated(r);
            this->Closedfile = true;
            return ;
        }

        if (CheckingStatusCode(Status_Code) == true && this->Error == false)
            this->ResponseMessageError(r);
        
        else if (this->Error == false)
        {
            OpeningFile(r.path);
            if (this->FinishedHeader == false)
                ResponseHeader(r);
            if (this->Error == true)
                return ;
            ResponseBody(r);
        }
        else
            return ;
    }
}