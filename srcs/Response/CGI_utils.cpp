#include "Response.hpp"


//return the parametre which needed by Execve function
char **Response::param_exec(std::string executable, std::string script)
{
    char **argv = new char*[3];
    size_t i = 0;

    argv[0] = new char[executable.length() + 1];
    argv[1] = new char[script.length() + 1];
    argv[2] = NULL;


    for(; i < executable.length();i++)
        argv[0][i] = executable[i];
    argv[0][i] = '\0';
    i = 0;
    for(; i < script.length();i++)
        argv[1][i] = script[i];
    argv[1][i] = '\0';
    return argv;
}



// ALLocation of Any element needed for Environment o execve() function
char *Response::envp_element(std::string str)
{
    char *env_el = NULL;
    env_el = new char[str.length() + 1];
    size_t i = 0;
    for(;i < str.length();i++)
        env_el[i] = str[i];
    env_el[i] = '\0';
    return env_el;
} 



// Construction of Environment needed by execve()
char **Response::env(request &r)
{
    char **envp = NULL;
    
    if (r.GeterMethod() == "POST")
        envp = new char*[14];
    else
        envp = new char*[12];


    if (GettingExtension(r.CGIPath) == ".py")
            envp[0] = envp_element("SCRIPT_NAME=" + r.PathExtensionCgi);
    if (GettingExtension(r.CGIPath) == ".php")
            envp[0] = envp_element("SCRIPT_NAME=" +  getlocalPath() + r.PathExtensionCgi);
    envp[1] = envp_element("REQUEST_METHOD=" + r.GeterMethod());
    envp[2] = envp_element("PATH_INFO=" + r.CGIPath);
    envp[3] = envp_element("SCRIPT_FILENAME=" + r.CGIPath);
    envp[4] = envp_element("QUERY_STRING=" + r.QueryString);
    envp[5] = envp_element("HTTP_HOST=" + r.Rhostname);
    envp[6] = envp_element("HTTP_PORT=" + r.Rport);
    envp[7] = envp_element("HTTP_USER_AGENT=" + r.map["User-Agent"]);
    envp[8] = envp_element("HTTP_COOKIE=" + r.map["Cookie"]);
    envp[9] = envp_element("REDIRECT_STATUS=200");
    envp[10] = envp_element("REMOTE_ADDR=" + r.Rhostname);
    if (r.GeterMethod() == "POST")
    {
        envp[11] = envp_element("CONTENT_TYPE=" + r.map["Content-Type"]);
        envp[12] = envp_element("CONTENT_LENGTH=" + my_to_string(LengthFile(r.path)));
        envp[13] = NULL;
    }
    else
        envp[11] = NULL;
    return (envp);
}