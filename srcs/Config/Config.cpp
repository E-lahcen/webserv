#include "Config.hpp"
#include <string>

Config::Config(const char* filePath)
{
    load(filePath);
}

Config::~Config(){}


void Config::load( const char* path)
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
        if (setSyntax(line)) 
            continue; // Skip empty lines, comments, and block delimiters

        line = trim_spaces(line);
        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos) 
        {
            std::string key = trim_spaces(line.substr(0, delimiterPos));
            std::string value = trim_spaces(line.substr(delimiterPos + 1));
            
            if (isValidKey(key))
                settings[key] = value;
            else
                throw std::runtime_error("Invalid key in configuration: " + key);
        }
        else if (line.substr(0, 8) == "location")
            configLocations.insert(parseLocation(line));
    }
    if (isValidBrackets())
        throw std::runtime_error("Invalid Brackets Syntax!");
}

bool    Config::isValidBrackets( ) const
{
    return (Brackets[0] == Brackets[1]);
}

bool    Config::setSyntax( std::string& line ) const
{
    if (line.empty() || line[0] == '#')
        return true;
    else if (line[0] == '{')
    {
        Brackets[0] += 1;
        return true;
    }
    else if (line[1] == '}')
    {
        Brackets[1] += 1;
        return true;
    }
    return false;
}


std::string Config::get(const std::string& key ) const 
{
    std::unordered_map<std::string, std::string>::const_iterator it = settings.find(key);
    if (it != settings.end())
        return it->second;
    throw std::runtime_error("Key not found in configuration: " + key);
}

Config::Location Config::getFromLocation( const Path& path ) const 
{
    std::unordered_map<Path, Config::Location>::const_iterator it = configLocations.find(path);
    std::cout << "umap.size = " << configLocations.size()  << " path = " << path << " ,conf path = " << configLocations.begin()->first << std::endl;
    if (it != configLocations.end())
        return it->second;
    throw std::runtime_error("Key not found in Location configuration: " + path);
}

bool Config::isValidKey(const std::string& key) const 
{
    for (size_t i = 0; i < (sizeof(validKeys) / sizeof(validKeys[0])); i++)
    {
        if (validKeys[i] == key)
            return true;
    }
    return false;
}

bool Config::isValidLocationKey( const std::string& key ) const 
{
    for (size_t i = 0; i < (sizeof(validLocationKeys) / sizeof(validLocationKeys[0])); i++)
    {
        if (validLocationKeys[i] == key)
            return true;
    }
    return false;
}

std::string Config::trim_spaces( const std::string&   str )
{
    std::size_t first = str.find_first_not_of(" \t");
    std::size_t last = str.find_last_not_of(" \t");
    if (first == std::string::npos || last == std::string::npos) {
        return "";
    }
    return str.substr(first, last - first + 1);
}

std::pair<Path, Config::Location> Config::parseLocation(const std::string& locationline)
{
    // Create a new Location object
    Config::Location location;

    // Parse the location block
    std::istringstream iss(locationline);
    Path locationPath;
    std::string block;
    iss >> locationPath;
    std::getline(iss, block);

    // Trim location path and block
    locationPath = trim_spaces(locationPath);
    block = trim_spaces(block);

    // Process the block content
    std::istringstream blockStream(block);
    std::string line;
    while (std::getline(blockStream, line, '\n'))
    {
        line = trim_spaces(line);
        if (line.empty() || line[0] == '#' || line[0] == '{')
            continue; // Skip empty lines and comments

        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos) 
        {
            std::string key = trim_spaces(line.substr(0, delimiterPos));
            std::string value = trim_spaces(line.substr(delimiterPos + 1));
            
            if (isValidLocationKey(key) && !value.empty())
            {

                // Set the corresponding parameters in the Location object
                if (key == "allow_methods") {
                    // Handle allow_methods setting
                    // You can parse and set the methods accordingly
                } else if (key == "redirect") {
                    // Handle redirect setting
                    // You can parse and set the redirection accordingly
                } else if (key == "root") {
                    // Handle root setting
                    location.root = value;
                } else if (key == "autoindex") {
                    // Handle autoindex setting
                    location.autoindex = (value == "on");
                } else if (key == "default") {
                    // Handle default setting
                    location.defaultFile = value;
                } else if (key == "upload") {
                    // Handle upload setting
                    location.uploadRoute = value;
                } else 
                    throw std::runtime_error("Invalid key in location block: " + key);
            }
            else
                throw std::runtime_error("Invalid key in configuration: " + key);
        }
    }
    return (std::pair<Path, Config::Location>(block, location));
}

// Location constructor initialisation
Config::Location::Location() : get(false), post(false), del(false), autoindex(false) {}


Config::Location::~Location(){}