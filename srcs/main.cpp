/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test_request_config.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 15:20:32 by ydahni            #+#    #+#             */
/*   Updated: 2023/06/21 17:35:38 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Config.hpp>

int main(int ac, char **av)
{
    try
    {
        (void)ac;
        Config config(av[1]);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading configuration: " << e.what() << std::endl;
    }
    // system("leaks webserv");
}