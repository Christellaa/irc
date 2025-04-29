/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/04/29 12:41:00 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool checkModeK(std::string word, Channel& channel)
{
	if (word.empty())
	{
		std::cout << "No password given" << std::endl;
		return false;
	}
	else if (!word.empty() && word != channel.getPassword())
	{
		std::cout << "Incorrect password" << std::endl;
		return false;
	}
	return true;
}

bool checkModeL(Channel& channel)
{
	return ((channel.getClients().size() < channel.getUserLimit()) || channel.getUserLimit() == 0);
}

bool checkModeI(Client& client, Channel& channel)
{
	if (client.findChannel(channel))
		return true;
	return false;
}

bool checkModes(std::string word, Channel& channel, Client& client)
{
	bool modeOk = true;
	if (!channel.getPassword().empty())
		modeOk = checkModeK(word, channel);
	std::cout << "mode Ok: " << modeOk << std::endl;
	if (channel.getUserLimit() != 0)
		modeOk = checkModeL(channel);
	std::cout << "mode Ok: " << modeOk << std::endl;
	if (channel.isInviteOnly())
		modeOk = checkModeI(client, channel);
	std::cout << "mode Ok: " << modeOk << std::endl;
	return modeOk;
}

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
			iss >> word;
			if (checkModes(word, *(*it), *client))
			{
				(*it)->getClients().push_back(client);
				std::cout << "Added " << client->getNickname() << " to channel " << (*it)->getName() << std::endl;
			}
			return;
		}
	}
	Channel* newChannel = new Channel(word);
	theServer.getChannels().push_back(newChannel);
	newChannel->getClients().push_back(client);
	newChannel->getOperators().push_back(client);
	std::cout << "Added " << client->getNickname() << " to NEW channel " << newChannel->getName() << std::endl;
}
