/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/05/09 13:44:46 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void removeClientFromChannel(std::string const& clientNickname, Channel& channel)
{
	ClientIterator client = channel.findClient(clientNickname);
	if (client != channel.getClients().end())
		channel.getClients().erase(client);
}

void removeOperator(std::string const& operatorNickname, Channel& channel)
{
	ClientIterator channelOperator = channel.findOperator(operatorNickname);
	if (channelOperator != channel.getOperators().end())
		channel.getOperators().erase(channelOperator);
}

void removeClientFromChannels(Client* client, Server& theServer)
{
	ChannelIterator currentChannel = theServer.getChannels().begin();
	while (currentChannel != theServer.getChannels().end())
	{
		removeClientFromChannel(client->getNickname(), *(*currentChannel));
		removeOperator(client->getNickname(), *(*currentChannel));
		if ((*currentChannel)->getClients().size() == 0)
		{
			delete *currentChannel;
			currentChannel = theServer.getChannels().erase(currentChannel);
		}
		else
		{
			if ((*currentChannel)->getOperators().size() == 0)
				(*currentChannel)->giveOperatorRights((*currentChannel)->getClients().begin());
			++currentChannel;
		}
	}
}

void quit(Client* client, Server& theServer)
{
	removeClientFromChannels(client, theServer);
	ClientIterator clientIt = theServer.findClient(client->getSocket());
	if (clientIt != theServer.getClients().end())
	{
		std::ostringstream oss;
		oss << client->getSocket();
		sendServerReply(*client, QUIT(client->getNickname(), oss.str()));
		close(client->getSocket());
		delete client;
		theServer.getClients().erase(clientIt);
	}
}
