/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/04/28 11:07:29 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void join(Client* client, Server& theServer, std::istringstream& iss)
{
	std::string word;
	iss >> word;
	word = word.substr(1);
	ChannelIterator it = theServer.getChannels().begin();
	ChannelIterator ite = theServer.getChannels().end();
	for (; it != ite; ++it)
	{
		if (word == (*it)->getName())
		{
			std::cout << "word: " << word << " + channel name: " << (*it)->getName() << std::endl;
			// check invite-only, etc
			(*it)->getClients().push_back(client);
			std::cout << "Added " << client->getNickname() << " to channel " << (*it)->getName() << std::endl;
			return;
		}
	}
	Channel* newChannel = new Channel(word);
	theServer.getChannels().push_back(newChannel);
	newChannel->getClients().push_back(client);
	newChannel->getOperators().push_back(client);
	std::cout << "Added " << client->getNickname() << " to NEW channel " << newChannel->getName() << std::endl;
}
