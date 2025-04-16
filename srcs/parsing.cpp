/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewu <jewu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:57:21 by jewu              #+#    #+#             */
/*   Updated: 2025/04/16 14:00:53 by jewu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Macros.hpp"
#include "parsing.hpp"

static bool is_valid_password(std::string const& password_str)
{
	if (password_str.empty())
		return FALSE;
	for (std::string::const_iterator it = password_str.begin(); it != password_str.end(); it++)
	{
		if (std::isspace(*it))
			return FALSE;
	}
	return TRUE;
}

static bool is_valid_port(std::string const& port_str)
{
	if (port_str.empty())
		return FALSE;
	for (std::string::const_iterator it = port_str.begin(); it != port_str.end(); it++)
	{
		if (!std::isdigit(static_cast<unsigned char>(*it)))
			return FALSE;
	}
	long port_number = std::strtol(port_str.c_str(), NULL, 10);
	return port_number >= 1024 && port_number <= 49151;
}

bool check_arguments(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << BOLD BORDEAUX "Error: invalid number of arguments, try ./ircserv <port> <password>" RESET << std::endl;
		return FALSE;
	}
	if (is_valid_port(argv[1]) == FALSE)
	{
		std::cerr << BOLD BORDEAUX "Error: invalid user port, try a number between 1024 and 49151" RESET << std::endl;
		return FALSE;
	}
	if (is_valid_password(argv[2]) == FALSE)
	{
		std::cerr << BOLD BORDEAUX "Error: invalid password, try a password like 'angrybots' with no spaces" RESET << std::endl;
		return FALSE;
	}
	std::cout << BOLD NEON_GREEN "YAY PARSING WORKS" RESET << std::endl;
	return TRUE;
}

void set_socket_non_blocking(int socketfd)
{
	int flags = fcntl(socketfd, F_GETFL, 0);
	fcntl(socketfd, F_SETFL, flags | O_NONBLOCK);
}
