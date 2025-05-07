/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:25:58 by jewu              #+#    #+#             */
/*   Updated: 2025/05/07 14:07:48 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void nick(Client& client, Server& theServer, std::istringstream& iss)
{
	std::string word;
	iss >> word;
	if (word.empty())
	{
		std::cout << "You didn't give a nickname" << std::endl;
		return;
	}
	if (word.length() > 8)
		word = word.substr(0, 8);
	client.setNickname(word);
	client.sameNickname(theServer);
	std::string welcome_msg = WELCOME(client.getNickname());
	// std::string welcome_msg = welcome_client(client.getNickname(), userPrefix(client.getNickname(), client.getUsername()));
	send(client.getSocket(), welcome_msg.c_str(), welcome_msg.length(), 0);
}
