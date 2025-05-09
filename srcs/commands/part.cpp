/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/05/09 14:59:13 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void part(Client& client, Server& theServer, std::istringstream& iss)
{
	std::string channelName;
	iss >> channelName;
	channelName = channelName.substr(1);

	ChannelIterator channel = theServer.findChannel(channelName);
	if (channel != theServer.getChannels().end())
	{
		removeClientFromChannel(client.getNickname(), *(*channel));
		removeOperator(client.getNickname(), *(*channel));
		if ((*channel)->getClients().size() == 0)
		{
			delete *channel;
			channel = theServer.getChannels().erase(channel);
		}
		else if ((*channel)->getOperators().size() == 0)
			(*channel)->giveOperatorRights((*channel)->getClients().begin());
		sendServerReply(client, PART(client.getNickname(), channelName));
	}
}
