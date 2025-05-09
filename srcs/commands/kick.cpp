/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:58:18 by jewu              #+#    #+#             */
/*   Updated: 2025/05/09 15:03:04 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

// KICK #test codespac0 :message de la mort

void kick(Client& client, Server& theServer, std::istringstream& iss)
{
	std::string channelName, clientToKick;
	iss >> channelName >> clientToKick;
	channelName = channelName.substr(1);
	std::cout << "channel: " << channelName << " client to kick: " << clientToKick << std::endl;
	// check if client is operator
	ChannelIterator channel = theServer.findChannel(channelName);
	std::cout << "SEARCHING" << std::endl;
	if (channel != theServer.getChannels().end())
	{
		// check if clientB is in channel
		std::cout << "FOUND CHANNEL" << std::endl;
		removeClientFromChannel(clientToKick, *(*channel));
		removeOperator(clientToKick, *(*channel));
		if ((*channel)->getClients().size() == 0)
		{
			delete *channel;
			channel = theServer.getChannels().erase(channel);
		}
		else if ((*channel)->getOperators().size() == 0)
			(*channel)->giveOperatorRights((*channel)->getClients().begin());
		std::string message;
		std::getline(iss, message);
		sendServerReply(client, KICK(client.getNickname(), channelName, clientToKick, message));
		std::cout << "KICKED" << std::endl;
	}
}
