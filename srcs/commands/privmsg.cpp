/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:02:32 by jewu              #+#    #+#             */
/*   Updated: 2025/05/07 11:01:15 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void privmsg(Client& client, Server& theServer, std::istringstream& iss)
{
	(void)client;
	std::string channelName;
	iss >> channelName;
	channelName = channelName.substr(1);
	std::string line;
	std::getline(iss, line);
	int pos = line.find(':');
	line = line.substr(pos + 1);

	ChannelIterator channel = theServer.findChannel(channelName);
	if (channel != theServer.getChannels().end())
	{
		ClientIterator it = (*channel)->getClients().begin();
		ClientIterator ite = (*channel)->getClients().end();
		for (; it != ite; ++it)
		{
			if (client.getSocket() == (*it)->getSocket())
				continue;
			send((*it)->getSocket(), line.c_str(), line.length(), 0);
			std::cout << "sending msg to: " << (*it)->getSocket() << std::endl;
		}
		return;
	}
}

// PRIVMSG #a :abc def hij
