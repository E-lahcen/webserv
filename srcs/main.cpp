#include "Config/Config.hpp"

int main() {
    try {
        Config config("Conf_files/config_file_template");
        std::string serverName = config.get("server_name");
        std::string listen = config.get("listen");
        std::string errorPage = config.get("error_page");
        std::string clientBodySizeMax = config.get("client_body_size_max");

        // Use the retrieved configuration settings
        std::cout << "Server Name: " << serverName << std::endl;
        std::cout << "Listen: " << listen << std::endl;
        std::cout << "Error Page: " << errorPage << std::endl;
        std::cout << "Client Body Size Max: " << clientBodySizeMax << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error loading configuration: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
