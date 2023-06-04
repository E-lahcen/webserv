#include <Config.hpp>
#include <Network.hpp>
#include <string>

Config::Config(const char *filePath)
{
    load(filePath);
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
    while (std::getline(file, line))
    {
        line = trim_spaces(line);
        if (setSyntax(line))
            continue; // Skip empty lines, comments, and block delimiters
        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos)
        {
            std::string key = trim_spaces(line.substr(0, delimiterPos));
            std::string value = trim_spaces(line.substr(delimiterPos + 1));

            if (isValidKey(key))
                configServers.back().setSettings(key, value);
            else
                throw std::runtime_error("Invalid key in configuration: " + key);
        }
        else if (line.substr(0, 8) == "location")
            configServers.back().setServerLocations(parseLocation(file, line));
        else
            throw std::runtime_error("Invalid \"Key=value syntax\" in configuration! " + line);
    }
    if (!isValidBrackets())
        throw std::runtime_error("Invalid Brackets Syntax!");
}

bool Config::isValidBrackets() const
{
    return (Brackets[0] == Brackets[1]);
}

bool Config::setSyntax(std::string &line)
{
    if (line == "server")
    {
        Server server;
        configServers.push_back(server);
        return true;
    }
    else if (line.empty() || line[0] == '#')
        return true;
    else if (line == "{")
    {
        Brackets[0] += 1;
        return true;
    }
    else if (line == "}")
    {
        Brackets[1] += 1;
        return true;
    }
    return false;
}

std::string Config::get(const std::string &key) const
{
    std::unordered_map<std::string, std::string> last_settings = configServers.back().getSettings();
    std::unordered_map<std::string, std::string>::const_iterator it = last_settings.find(key);
    if (it != last_settings.end())
        return it->second;
    throw std::runtime_error("Key not found in configuration: " + key);
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

    std::string line;
    while (std::getline(ifile, line))
    {
        line = trim_spaces(line);
        if (setSyntax(line) && line != "}")
            continue; // Skip empty lines, comments, and block delimiters

        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos)
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
                    location.root = value;
                else if (key == "autoindex")
                    location.autoindex = (value == "on");
                else if (key == "default")
                    location.defaultFile = value;
                else if (key == "upload")
                    location.uploadRoute = value;
                else if (key == "cgi")
                    location.cgi.insert(parseCgi(value));
            }
            else
                throw std::runtime_error("Invalid key in location block: " + key);
        }
        else if (line == "}")
            break;
    }
    return (std::pair<Path, Server::Location>(locationPath, location));
}

std::pair<StatusNbr, Path> Config::parseRedirection(const std::string &line)
{
    std::istringstream iss(line);
    Path value;
    std::string stat;
    iss >> stat;
    std::getline(iss, value);
    if (stat.empty() || value.empty())
        throw std::runtime_error("Invalid Redirection");
    return (std::pair<StatusNbr, Path>(std::stoi(stat), value));
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
