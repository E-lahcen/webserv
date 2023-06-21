/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 11:55:05 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/21 04:30:31 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils.hpp>

void throwErrnoException(const std::string &context)
{
    throw std::runtime_error(context);
}

void makeFDNonBlock(int fd)
{

    if (fcntl(fd, F_SETFD, O_NONBLOCK) == -1)
        throwErrnoException("failed to make FD non-blocking");
}

bool is_number(const std::string &s)
{
    for (std::string::const_iterator it = s.begin(); it != s.end(); it++)
        if (!std::isdigit(*it))
            return false;
    return true;
}

void check_host_name(int hostname) { //This function returns host name for local computer
   if (hostname == -1)
      throw std::runtime_error("Gethostbyname() error during initilizing local IP address when checking hostname");

}
void check_host_entry(struct hostent * hostentry) { //find host info from host name
   if (hostentry == NULL){
      throw std::runtime_error("Gethostbyname() error during initilizing local IP address");
   }
}

std::string getLocalIpAddress()
{
    char host[256];
    char *IP;
    struct hostent *host_entry;
    int hostname;
    hostname = gethostname(host, sizeof(host)); //find the host name
    check_host_name(hostname);
    host_entry = gethostbyname(host); //find host information
    check_host_entry(host_entry);
    IP = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0])); //Convert into IP string
    return IP;
}

std::string    getlocalPath( )
{
    const int bufferSize = 4096; // Maximum path length
    char buffer[bufferSize];
    
    if (getcwd(buffer, bufferSize) != NULL)
        return buffer;
    return "";
}

std::string getRealPath(const std::string& input ) 
{
    std::string result = input;
    std::string  localPath = getlocalPath();
    size_t pos = 0;
    
    while ((pos = result.find("{PATH}", pos)) != std::string::npos) {
        result.replace(pos, 6, localPath);
        pos += localPath.length();
    }
    return result;
}