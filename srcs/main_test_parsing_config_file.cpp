#include "Config.hpp"

int main(int ac, char **av)
{
    try
    {
        (void)ac;
        Config config(av[1]);
        std::string serverName = config.configServers.back().serverName;
        std::string hostname = config.configServers.back().hostname;
        std::string errorPage = config.configServers.back().errorPage[405];
        size_t clientBodySizeMax = config.configServers.back().clientBodySizeMax;
        bool autoindex = config.getFromLocation("/").autoindex;
        Path defaultF = config.getFromLocation("/").defaultFile;
        bool get = config.getFromLocation("/").get;
        bool post = config.getFromLocation("/").post;
        bool del = config.getFromLocation("/").del;
        CgiPair cgi = config.getFromLocation("/").cgi;

        // Use the retrieved configuration settings
        std::cout << "Server Name: " << serverName << std::endl;
        std::cout << "Hostname: " << hostname << std::endl;
        std::cout << "Error Page: " << errorPage << std::endl;
        std::cout << "Client Body Size Max: " << clientBodySizeMax << std::endl;
        std::cout << "Location : / , autoindex = " << autoindex << std::endl;
        std::cout << "Location : / , default = " << defaultF << std::endl;
        std::cout << "Location : / , get = " << get << " post = " << post << " del = " << del << std::endl;
        std::cout << "Location : / , cgi[.py] = " << cgi[".py"] << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading configuration: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
