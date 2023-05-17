#include "Config.hpp"

Config::Config(const std::string& filePath)
{
    load(filePath);
}

Config::~Config(){}


void Config::load(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file) {
            throw std::runtime_error("Failed to open configuration file: " + filePath);
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#' || line[0] == '}' || line[0] == '{') {
                continue; // Skip empty lines, comments, and block delimiters
            }

            size_t delimiterPos = line.find(' ');
            if (delimiterPos != std::string::npos) {
                std::string key = line.substr(0, delimiterPos);
                std::string value = line.substr(delimiterPos + 1);
                settings[key] = value;
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

bool Config::isValidKey(const std::string& key) const {
    for (size_t i = 0; i < (sizeof(validKeys) / sizeof(validKeys[0])); i++)
    {
        if (validKeys[i] == key)
            return true;
    }
    return false;
}