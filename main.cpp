/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:25:58 by jewu              #+#    #+#             */
/*   Updated: 2025/04/15 15:07:54 by cde-sous         ###   ########.fr       */
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
		int port = std::atoi(argv[1]);
		Server theServer(port, argv[2]);
		theServer.launch_angrybots_server();
		// epoll -> y mettre server's socket
		// boucle d'ecoute du server
	}catch(const std::exception& e)
	{
		std::cout << "Exception caught: " << e.what() << std::endl;
	}
	
	return EXIT_SUCCESS;
}
