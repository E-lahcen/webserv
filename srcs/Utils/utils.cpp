/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 11:55:05 by lelhlami          #+#    #+#             */
/*   Updated: 2023/05/29 16:55:00 by lelhlami         ###   ########.fr       */
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