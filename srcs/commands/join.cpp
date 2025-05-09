/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/05/09 14:01:14 by codespace        ###   ########.fr       */
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
	return (((int)channel.getClients().size() < channel.getUserLimit()) || channel.getUserLimit() == 0);
}

bool checkModeI(Client& client, Channel& channel)
{
	if (client.findInvitedChannel(channel))
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
	ChannelIterator channel = theServer.findChannel(word);
	if (channel != theServer.getChannels().end())
	{
		std::cout << "word: " << word << " + channel name: " << (*channel)->getName() << std::endl;
		iss >> word;
		if (checkModes(word, *(*channel), *client))
		{
			(*channel)->getClients().push_back(client);
			std::cout << "Added " << client->getNickname() << " to channel " << (*channel)->getName() << std::endl;
			sendServerReply(*client, JOIN(client->getNickname(), (*channel)->getName()));
		}
		else
			std::cout << "Error in checkmodes, JOIN fail" << std::endl;
		return;
	}
	if (word.length() > MAX_CHAR_CHANNEL)
	{
		std::cout << "Channel name must be maximum " << MAX_CHAR_CHANNEL << " characters" << std::endl;
		return;
	}
	// check if MAX_CHANNELS reached
	Channel* newChannel = new Channel(word);
	theServer.getChannels().push_back(newChannel);
	newChannel->getClients().push_back(client);
	newChannel->getOperators().push_back(client);
	std::cout << "Added " << client->getNickname() << " to NEW channel " << newChannel->getName() << std::endl;

	sendServerReply(*client, JOIN(client->getNickname(), newChannel->getName()));
}
