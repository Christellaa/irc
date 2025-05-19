/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:25:58 by jewu              #+#    #+#             */
/*   Updated: 2025/05/19 13:39:13 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

static bool check_same_nickame(Client &client, Server &theServer, std::string &newNick)
{
	ClientIterator it = theServer.getClients().begin();
	ClientIterator ite = theServer.getClients().end();
	for (; it != ite; ++it)
	{
		if (newNick == (*it)->getNickname())
		{
			saveServerReply(client, ERR_NICKNAMEINUSE(newNick, " :Nickname is already in use."));
			return true;
		}
	}
	return false;
}

void nick(Client &client, Server &theServer, std::istringstream &iss)
{
	std::string word;
	iss >> word;
	if (word.empty())
	{
		saveServerReply(client, ERR_ERRONEUSNICKNAME(client.getNickname(), word, "No nickname given"));
		return;
	}
	if (word.length() > 8)
		word = word.substr(0, 8);
	if (hasForbiddenChars(word, "client"))
	{
		saveServerReply(client, ERR_ERRONEUSNICKNAME(client.getNickname(), word, "Nickname has invalid characters"));
		return;
	}
	std::string oldNickname = client.getNickname();
	if (!check_same_nickame(client, theServer, word))
	{
		client.setNickname(word);
		client.sameNickname(theServer);
		saveServerReply(client, RPL_SAVENICK(oldNickname, client.getNickname()));
	}
}
