/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Macros.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:38:01 by jewu              #+#    #+#             */
/*   Updated: 2025/05/07 14:12:20 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Macros.hpp"

std::string userPrefix(std::string nickname, std::string username)
{
	return nickname + "!" + username + "@localhost";
}

// std::string welcome_client(std::string nickname, std::string prefix)
// {
// 	(void)prefix;
// 	return ":ircserv 001 " + nickname + " :Welcome, " + nickname + "\r\n";
// }

// std::string messageToSend(std::string const& nickname, std::string const& channelName, std::string const& message)
// {
// 	return ":" + nickname + " PRIVMSG #" + channelName + " :" + message + "\r\n";
// }

const char* SignalQuit::what() const throw(){
	return BOLD BLUE "\nQuitting server..." RESET;
}

std::string intToString(int number)
{
	std::ostringstream oss;
	oss << number;
	return oss.str();
}
