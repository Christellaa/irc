/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:25:58 by jewu              #+#    #+#             */
/*   Updated: 2025/05/14 11:26:36 by cde-sous         ###   ########.fr       */
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
			sendServerReply(client, ERR_NICKNAMEINUSE(newNick, " :Nickname is already in use."));
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
		sendServerReply(client, ERR_ERRONEUSNICKNAME(client.getNickname(), word, "No nickname given"));
		return;
	}
	if (word.length() > 8)
		word = word.substr(0, 8);
	if (hasForbiddenChars(word, "client"))
	{
		sendServerReply(client, ERR_ERRONEUSNICKNAME(client.getNickname(), word, "Nickname has invalid characters"));
		return;
	}
	std::string oldNickname = client.getNickname();
	if (!check_same_nickame(client, theServer, word))
	{
		client.setNickname(word);
		client.sameNickname(theServer);
		sendServerReply(client, RPL_SAVENICK(oldNickname, client.getNickname()));
	}
}
