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
        if (line.empty() || line[0] == '#' || line[0] == '}' || line[0] == '{') 
            continue; // Skip empty lines, comments, and block delimiters

        line = trim_spaces(line);
        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos) 
        {
            std::string key = trim_spaces(line.substr(0, delimiterPos));
            std::string value = trim_spaces(line.substr(delimiterPos + 1));
            
            if (isValidKey(key))
                settings[key] = value;
            else if (isValidLocationKey(key))
                configLocations[key] = value;
            else
                throw std::runtime_error("Invalid key in configuration: " + key);
        }
    }
}

std::string Config::get(const std::string& key) const 
{
    std::unordered_map<std::string, std::string>::const_iterator it = settings.find(key);
    if (it != settings.end()) {
        return it->second;
    }
    throw std::runtime_error("Key not found in configuration: " + key);
}

bool Config::isValidKey(const std::string& key) const 
{
    for (size_t i = 0; i < (sizeof(validKeys) / sizeof(validKeys[0])); i++)
    {
        if (validKeys[i] == key)
            return true;
    }
    for (size_t i = 0; i < (sizeof(validLocationKeys) / sizeof(validLocationKeys[0])); i++)
    {
        if (validLocationKeys[i] == key)
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

std::string trim_spaces( const std::string&   str )
{
    std::size_t first = str.find_first_not_of(" \t");
    std::size_t last = str.find_last_not_of(" \t");
    if (first == std::string::npos || last == std::string::npos) {
        return "";
    }
    return str.substr(first, last - first + 1);
}

Config::Location::~Location(){}