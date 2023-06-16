/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 11:55:46 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/16 12:12:27 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Config.hpp>
#include <Network.hpp>

static size_t Brackets[2] = {0, 0};

void throwErrnoException(const std::string &context);
void makeFDNonBlock(int fd);
bool is_number(const std::string &s);
std::string getLocalIpAddress();
std::string    getlocalPath( );
std::string getRealPath(const std::string& input );

