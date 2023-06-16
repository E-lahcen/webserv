#pragma once

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <request.hpp>
#include <ctime>
#include <string.h>
#include <unistd.h>
#include <cstring>


class request;

class Response{
    public:
    Response();





    //Utils
    std::string Timing();
    std::string my_to_string(size_t n);
    size_t LengthFileError(std::string filename);
    size_t LengthFile(std::string filename);
    bool CheckingStatusCode(int status_code);
    std::string GettingContentType(std::string path);
    std::string FindingExten(std::string extension);

    int Status_Code;
    int fd;
    int OpenedFd;


    //bools

    bool FinishedHeader;
    bool Closedfile;
    bool Opening;


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


    // void CasesStatusline(int fd, request &r);
    // void ResponseHeader(request &r);
    // void Timing();
    // void ResponseBody(request &r);
    // long LengthFile(std::string filename);
    // std::string HtmlContent(request &r);
    // std::string ContentLengthFile(request &r);
    // std::string ContentTypeFile(request &r);
};

#endif
