/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:25:58 by jewu              #+#    #+#             */
/*   Updated: 2025/05/09 13:25:04 by codespace        ###   ########.fr       */
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
	std::string oldNickname = client.getNickname();
	client.setNickname(word);
	client.sameNickname(theServer);
	sendServerReply(client, RPL_SAVENICK(oldNickname, client.getNickname()));
}
