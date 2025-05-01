/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:02:32 by jewu              #+#    #+#             */
/*   Updated: 2025/05/01 13:28:41 by codespace        ###   ########.fr       */
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

	ChannelIterator currentChannel = theServer.getChannels().begin();
	ChannelIterator lastChannel = theServer.getChannels().end();
	for (; currentChannel != lastChannel; ++currentChannel)
	{
		if ((*currentChannel)->getName() == channelName)
		{
			ClientIterator it = (*currentChannel)->getClients().begin();
			ClientIterator ite = (*currentChannel)->getClients().end();
			for (; it != ite; ++it)
			{
				// don't send to client who sent
				send((*it)->getSocket(), line.c_str(), line.length(), 0);
				std::cout << "sending msg to: " << (*it)->getSocket() << std::endl;
			}
			return;
		}
	}
}

// PRIVMSG #a :abc def hij
