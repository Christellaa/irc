/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:02:32 by jewu              #+#    #+#             */
/*   Updated: 2025/05/09 13:24:57 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void privmsg(Client& client, Server& theServer, std::istringstream& iss)
{
	std::string channelName;
	iss >> channelName;
	channelName = channelName.substr(1);
	std::string message;
	std::getline(iss, message);
	int pos = message.find(':');
	message = message.substr(pos + 1);

	ChannelIterator channel = theServer.findChannel(channelName);
	if (channel != theServer.getChannels().end())
	{
		ClientIterator it = (*channel)->getClients().begin();
		ClientIterator ite = (*channel)->getClients().end();
		for (; it != ite; ++it)
		{
			if (client.getSocket() == (*it)->getSocket())
				continue;
			std::string line = PRIVMSG(client.getNickname(), (*channel)->getName(), message);
			send((*it)->getSocket(), line.c_str(), line.size(), 0);
		}
	}
}
