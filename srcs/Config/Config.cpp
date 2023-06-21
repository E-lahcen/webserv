#include <Config.hpp>
#include <Network.hpp>
#include <string>


Config::Config(const char *filePath)
{
    load(filePath);
    createDefaultServer();
    std::cout << "Local Ip adress for Default Server : " << configServers.back().hostname << std::endl;
    std::cout << "Connected Servers : " << configServers.size() << std::endl;
    Network::initServersSockets(configServers);
}

Config::~Config() {}

void Config::load(const char *path)
{

    if (!path)
        throw std::runtime_error("Include a config file!");
    std::string filePath = path;
    std::ifstream file(filePath);

    if (!file)
        throw std::runtime_error("Failed to open configuration file: " + filePath);

    std::string line;
    bool    validServerCheck            = false;
    bool    validOpenBracketLocation    = true;
    while (std::getline(file, line))
    {
        line = trim_spaces(line);
        if (setSyntax(line, validServerCheck, validOpenBracketLocation))
            continue; // Skip empty lines, comments, and block delimiters
        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos && validServerCheck && validOpenBracketLocation)
        {
            std::string key = trim_spaces(line.substr(0, delimiterPos));
            std::string value = trim_spaces(line.substr(delimiterPos + 1));

            if (value.empty())
                throw std::runtime_error("Invalid value in configuration for the key : " + key);
            else if (isValidKey(key))
            {
                configServers.back().requiredKeysList.remove(key);
                configServers.back().setSettings(key, value);
            }
            else
                throw std::runtime_error("Invalid key in configuration: " + key);
        }
        else if (line.substr(0, 8) == "location" && validServerCheck)
        {
            configServers.back().requiredKeysList.remove("location");
            configServers.back().setServerLocations(parseLocation(file, line));
        }
        else
            throw std::runtime_error("Invalid syntax in configuration!");
    }
    file.close();
    checkControl();
}

bool Config::isValidBrackets() const
{
    return (Brackets[0] == Brackets[1]);
}

bool Config::setSyntax(std::string &line, bool  &resetServerCheck, bool  &validOpenBracketLocation)
{
    if (line == "server")
    {
        Server server;
        resetServerCheck = true;
        server.requiredKeysList.remove(line);
        configServers.push_back(server);
        return true;
    }
    else if (line.empty() || line[0] == '#')
        return true;
    else if (line == "{" && resetServerCheck)
    {
        validOpenBracketLocation = true;
        Brackets[0] += 1;
        return true;
    }
    else if (line == "}" && resetServerCheck)
    {
        Brackets[1] += 1;
        resetServerCheck = false;
        return true;
    }
    return false;
}

Server::Location Config::getFromLocation(const Path &path) const
{
    // Server last_server = configServers.back();
    std::unordered_map<Path, Server::Location> last_server_location = configServers.back().getServerLocations();
    std::unordered_map<Path, Server::Location>::const_iterator it = last_server_location.find("/");
    if (it != last_server_location.end())
        return it->second;
    throw std::runtime_error("Key not found in Location configuration: " + path);
}

bool Config::isValidKey(const std::string &key) const
{
    for (size_t i = 0; i < (sizeof(validKeys) / sizeof(validKeys[0])); i++)
    {
        if (validKeys[i] == key)
            return true;
    }
    return false;
}

bool Config::isValidLocationKey(const std::string &key) const
{
    for (size_t i = 0; i < (sizeof(validLocationKeys) / sizeof(validLocationKeys[0])); i++)
    {
        if (validLocationKeys[i] == key)
            return true;
    }
    return false;
}

std::string Config::trim_spaces(const std::string &str)
{
    std::size_t first = str.find_first_not_of(" \t");
    std::size_t last = str.find_last_not_of(" \t");
    if (first == std::string::npos || last == std::string::npos)
    {
        return "";
    }
    return str.substr(first, last - first + 1);
}

std::pair<Path, Server::Location> Config::parseLocation(std::ifstream &ifile, std::string &locationline)
{
    Server::Location location;

    // Parse the location path
    std::istringstream iss(locationline);
    Path locationPath;
    iss >> locationPath;
    std::getline(iss, locationPath);

    locationPath = trim_spaces(locationPath);
    if (locationPath[0] != '/')
        throw std::runtime_error("Invalid Location Path!");

    std::string line;
    bool    validServerCheck            = true;
    bool    validOpenBracketLocation    = false;
    while (std::getline(ifile, line))
    {
        line = trim_spaces(line);
        if (setSyntax(line, validServerCheck, validOpenBracketLocation) && line != "}")
            continue; // Skip empty lines, comments, and block delimiters

        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos && validOpenBracketLocation)
        {
            std::string key = trim_spaces(line.substr(0, delimiterPos));
            std::string value = trim_spaces(line.substr(delimiterPos + 1));

            if (isValidLocationKey(key) && !value.empty())
            {
                // Set the corresponding parameters in the Location object
                if (key == "allow_methods")
                    parseMethods(location, value);
                else if (key == "redirect")
                    location.redirection = parseRedirection(value);
                else if (key == "root")
                    location.root = getRealPath(value);
                else if (key == "autoindex")
                    location.autoindex = parseAutoindex(value);
                else if (key == "default")
                    location.defaultFiles = parseDefaultFile(value);
                else if (key == "upload")
                    location.uploadRoute = value;
                else if (key == "cgi")
                    location.cgi.insert(parseCgi(value));
                location.requiredLocationKeysList.remove(key);
            }
            else
                throw std::runtime_error("Invalid key in location block: " + key);
        }
        else if (line == "}")
            break;
        else
            throw std::runtime_error("Invalid Location Bracket syntax!");
    }
    if (!location.requiredLocationKeysList.empty())
        throw std::runtime_error("Non complete location settings, complete all keys needed!\nRequired location settings : allow_methods, root");

    return (std::pair<Path, Server::Location>(locationPath, location));
}

std::pair<StatusNbr, Path> Config::parseRedirection(const std::string &line)
{
    std::istringstream iss(line);
    Path value;
    short stat;
    iss >> stat;
    std::getline(iss, value);
    value = trim_spaces(value);
    if (!stat || value.empty())
        throw std::runtime_error("Invalid Redirection");
    return (std::pair<StatusNbr, Path>(stat, value));
}

FilePaths   Config::parseDefaultFile( std::string& value )
{
    std::stringstream ss(value);
    std::string word;
    std::vector<Path>   vec;

    while (ss >> word)
    {
        word = trim_spaces(word);
        vec.push_back(word);
        std::cout << " path = " << word << "\t";
    }
    std::cout << std::endl;
    if (vec.empty())
        throw std::runtime_error("Invalid configuration : default path must be filled!");
    return vec;
}

void Config::parseMethods(Server::Location &location, const std::string &value)
{
    std::stringstream ss(value);
    std::string word;

    while (ss >> word)
    {
        (word == "GET") ? location.get = true : (word == "POST") ? location.post = true
                                            : (word == "DELETE") ? location.del = true
                                                                 : throw std::runtime_error("Invalid Method : " + word);
    }
}

std::pair<Extension, Path> Config::parseCgi(const std::string &value)
{
    std::stringstream ss(value);
    Extension extension;
    Path path;

    ss >> extension;
    if (extension[0] != '.')
        throw std::runtime_error("Invalid CGI extension!");
    ss >> path;
    if (path[0] != '/')
        throw std::runtime_error("Invalid CGI path!");
    return std::pair<Extension, Path>(extension, path);
}

bool    Config::parseAutoindex( std::string& value )
{
    bool val;

    (value == "on" ) ? val = true : (value == "off") ? val = false :  throw std::runtime_error("Invalid auto index value : " + value); 
    return val;
}

void	Config::createDefaultServer()
{
    Server      serv;
    Server::Location    loc;

    serv.serverName = "Default Server";
    serv.hostname = getLocalIpAddress();
    serv.port = "8080";
    serv.errorPage.insert(std::make_pair(400, "/error_page/400.html"));
    serv.errorPage.insert(std::make_pair(403, "/error_page/403.html"));
    serv.errorPage.insert(std::make_pair(404, "/error_page/404.html"));
    serv.errorPage.insert(std::make_pair(405, "/error_page/405.html"));
    serv.errorPage.insert(std::make_pair(409, "/error_page/409.html"));
    serv.errorPage.insert(std::make_pair(413, "/error_page/413.html"));
    serv.errorPage.insert(std::make_pair(414, "/error_page/414.html"));
    serv.errorPage.insert(std::make_pair(500, "/error_page/500.html"));
    serv.errorPage.insert(std::make_pair(501, "/error_page/501.html"));
    serv.errorPage.insert(std::make_pair(505, "/error_page/505.html"));

    // Init Location for Default Server
    loc.route = "/";
    loc.get = true;
    loc.root = getlocalPath() + "/root";
    loc.defaultFiles.push_back("/index.html");

    serv.setServerLocations(std::make_pair("/", loc));

    configServers.push_back(serv);
}

bool    Config::checkRequiredKeys()
{
    for(std::vector<Server>::iterator it = configServers.begin(); it != configServers.end(); it++)
    {
        if (!it->requiredKeysList.empty())
            return false;
    }
    return true;
}

void    Config::checkControl()
{
    size_t  cnt;

    if (!checkRequiredKeys())
        throw std::runtime_error("Non complete settings, complete all keys needed!\nRequired settings : server, listen, client_body_size_max, location.");
    if (!configServers.size())
        throw std::runtime_error("Error in config file : Create at least one server!");
    if (!isValidBrackets())
        throw std::runtime_error("Invalid Brackets Syntax!");
    cnt = configServers.size();

    for(std::vector<Server>::iterator it = configServers.begin(); it != configServers.end(); it++)
    {
        cnt += it->getServerLocations().size();
        if (it->getServerLocations().find("/") == it->getServerLocations().end())
            throw std::runtime_error("Invalid location set, you need to add a / location!");
    }
    if (Brackets[0] != cnt) //Check if the number of the brackets pair equal to servers+location
        throw std::runtime_error("Invalid Syntax!");
}