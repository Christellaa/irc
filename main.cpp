/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewu <jewu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:25:58 by jewu              #+#    #+#             */
/*   Updated: 2025/04/14 16:08:12 by jewu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "parsing.hpp"

int main(int argc, char **argv)
{
	if (check_arguments(argc, argv) == FALSE)
		return EXIT_FAILURE;
	try
	{
		Server theServer(argv[1], argv[2]);
		theServer.launch_angrybots_server();
	}catch(const std::exception& e)
	{
		std::cout << "Exception caught: " << e.what() << std::endl;
	}
	
	return EXIT_SUCCESS;
}