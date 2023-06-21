#include "Response.hpp"

// cgi
void Response::CGI(request &r)
{
    std::string name = "/tmp/" + GetRandomName() + ".html";
    this->OPennedFdCGI = open(name.c_str(), O_CREAT | O_RDWR ,0777);
    if (r.GeterMethod() == "POST")
    {
        this->OpenedFd = open(r.path.c_str(), O_RDONLY);
        if (OpenedFd == -1)
        {
            close(OPennedFdCGI);
            close(OpenedFd);
            ErrorHandling(r);
            return ;
        }
    }
    if (OPennedFdCGI == -1)
    {
        close(OpenedFd);
        close(OPennedFdCGI);
        ErrorHandling(r);
        return ;
    }

    this->child_pid = fork(); 

    if (child_pid == -1)
    {
        close(OPennedFdCGI);
        close(OpenedFd);
        ErrorHandling(r);
        return ;
    }

    if (child_pid == 0)
        ChildProcess(r);
    if (r.GeterMethod() == "POST")
    {
        storpath = r.path;
        close(OpenedFd);
    }
    close(OPennedFdCGI);
    r.path = name;
}


// Handling Child process with execution of script
void Response::ChildProcess(request &r)
{
    char **argsp;
    char **envp = env(r);

    if (GettingExtension(r.CGIPath) == ".py")
        argsp = param_exec(r.PathExtensionCgi, r.CGIPath);
    if (GettingExtension(r.CGIPath) == ".php")
        argsp = param_exec(getlocalPath() + r.PathExtensionCgi, r.CGIPath);

    if (r.GeterMethod() == "POST")
        dup2(this->OpenedFd , 0);

    dup2(this->OPennedFdCGI , 1);

    if(execve(argsp[0], argsp, envp) == -1)
    {
        ErrorHandling(r);
        exit(-1);
    }
}



// Wait for child to finish Execution
pid_t Response::CheckChildPid()
{
    this->wait_child = waitpid(this->child_pid, NULL, WNOHANG);
    return (this->wait_child);
}