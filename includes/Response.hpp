#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <request.hpp>
#include <ctime>
#include <string.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>


class request;

class Response{
    public:

    Response();


    int Status_Code;
    int fd;
    int OpenedFd;
    int OPennedFdCGI;
    pid_t child_pid;
    pid_t wait_child;
    bool wait;
    std::string HeaderCGI;

    std::string storpath;


    //Utils
    std::string Timing();
    std::string my_to_string(size_t n);
    size_t LengthFileError(std::string filename);
    size_t LengthFile(std::string filename);
    bool CheckingStatusCode(int status_code);
    std::string GettingContentType(std::string path);
    std::string FindingExten(std::string extension);
    void ErrorHandling(request &r);
    std::string GettingExtension(std::string path);

    

    //bools

    bool FinishedHeader;
    bool Closedfile;
    bool Opening;
    bool Error;

    //Responsse
    void ResponseMessage(request &r);




    //Response Error
    std::string error;
    void ResponseMessageError(request &r);
    void ResponseHeaderError(request &r);
    std::string ContentTypeFileError();
    std::string ContentLengthFileError(request &r);
    std::string CasesStatuslineError(request &r);
    void OpeningFileError(std::string path);
    void ResponseBodyError();
    void PrintErrors();
    void PrintHtmlError(std::string errorMessage);



    // Response Message
    std::string ContentLengthFile(request &r);
    std::string ContentTypeFile(request &r);
    void ResponseHeader(request &r);
    void OpeningFile(std::string path);
    void ResponseBody(request &r);
    std::string CasesStatusline(request &r);
    void HeaderOfRedirect(request &r);
    void HeaderAndBodyOfCreated(request &r);


    //CGI
    void CGI(request &r);
    pid_t CheckChildPid();
    void ChildProcess(request &r);
    

    //CGI_ustils
    char *envp_element(std::string str);
    char **param_exec(std::string executable, std::string script);
    char **env(request &r);
    std::string HeaderOFCGI(request &r);
    bool HeaderInCaseError(request &r);
    bool HeaderInCaseErrorCGI(request &r);
    void HeaderOfDelete(request &r);
};

#endif
