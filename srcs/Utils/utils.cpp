/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 11:55:05 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/06 11:02:58 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils.hpp>

void throwErrnoException(const std::string &context)
{

    std::string errorMsg = context + ": ";
    errorMsg += std::strerror(errno);
    throw std::runtime_error(errorMsg);
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