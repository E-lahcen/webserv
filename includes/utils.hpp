/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 11:55:46 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/07 10:55:22 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Config.hpp>
#include <Network.hpp>

static short Brackets[2] = {0, 0};

void throwErrnoException(const std::string &context);
void makeFDNonBlock(int fd);
bool is_number(const std::string &s);