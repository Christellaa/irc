/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/04/29 14:02:07 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void removeClientFromChannel(Client* clientToRemove, ChannelIterator channel)
{
	ClientIterator currentClient = (*channel)->getClients().begin();
	ClientIterator lastClient = (*channel)->getClients().end();
	for (; currentClient != lastClient; ++currentClient)
	{
		if (clientToRemove->getSocket() == (*currentClient)->getSocket())
		{
			currentClient = (*channel)->getClients().erase(currentClient);
			break;
		}
	}
}

void removeOperator(Client* clientToRemove, ChannelIterator channel)
{
	ClientIterator currentOperator = (*channel)->getOperators().begin();
	ClientIterator lastOperator = (*channel)->getOperators().end();
	for (; currentOperator != lastOperator; ++currentOperator)
	{
		if (clientToRemove->getSocket() == (*currentOperator)->getSocket())
		{
			currentOperator = (*channel)->getOperators().erase(currentOperator);
			break;
		}
	}
}

void removeClientFromChannels(Client* client, Server& theServer)
{
	ChannelIterator currentChannel = theServer.getChannels().begin();
	while (currentChannel != theServer.getChannels().end())
	{
		removeClientFromChannel(client, currentChannel);
		removeOperator(client, currentChannel);
		if ((*currentChannel)->getClients().size() == 0)
		{
			delete *currentChannel;
			currentChannel = theServer.getChannels().erase(currentChannel);
		}
		else
		{
			(*currentChannel)->giveOperatorRights((*currentChannel)->getClients().begin());
			++currentChannel;
		}
	}
}

void quit(Client* client, Server& theServer)
{
	ClientIterator it = theServer.getClients().begin();
	ClientIterator ite = theServer.getClients().end();
	removeClientFromChannels(client, theServer);
	for (; it != ite; ++it)
	{
		if (client->getSocket() == (*it)->getSocket())
		{
			close(client->getSocket());
			delete client;
			theServer.getClients().erase(it);
			break;
		}
	}
}
