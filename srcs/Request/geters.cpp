/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geters.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydahni <ydahni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 21:52:04 by ydahni            #+#    #+#             */
/*   Updated: 2023/06/04 22:03:11 by ydahni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <request.hpp>


//method Geters

std::string request::GeterMethod() const
{
    return (this->method);
}

std::string request::GeterUri() const
{
    return (this->uri);
}

std::string request::GeterVersion() const
{
    return (this->version);
}

std::map<std::string, std::string> request::GeterHeader() const
{
    return (this->map);
}


void request::SetStatutCode(int statut)
{
    this->StatutCode = statut;
}

int request::GetStatutCode() const
{
    return (this->StatutCode);
}

