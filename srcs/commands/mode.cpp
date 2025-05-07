/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/05/07 10:29:22 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void handleO(std::string const& option, Channel& channel, bool isPositive)
{
	if (option.empty())
		return;
	if (isPositive)
	{
		ClientIterator client = channel.findClient(option);
		if (client != channel.getClients().end())
		{
			channel.giveOperatorRights(client);
			std::cout << (*client)->getNickname() << " changed " << channel.getName() << " to +o" << std::endl;
			return;
		}
		std::cout << option << " isn't part of " << channel.getName() << std::endl;
	}
	else
	{
		ClientIterator channelOperator = channel.findOperator(option);
		if (channelOperator != channel.getOperators().end())
		{
			channel.removeOperatorRights(*(*channelOperator));
			std::cout << (*channelOperator)->getNickname() << " changed " << channel.getName() << " to -o" << std::endl;
			return;
		}
		std::cout << option << " isn't an operator of " << channel.getName() << std::endl;
	}
}

void handleI(std::string const& option, Channel& channel, bool isPositive)
{
	(void)option;
	if (isPositive)
		channel.setInviteOnly(true);
	else
		channel.setInviteOnly(false);
	std::cout << "changed +i -i" << std::endl;
}

void handleT(std::string const& option, Channel& channel, bool isPositive)
{
	(void)option;
	if (isPositive)
		channel.setTopicScope(true);
	else
		channel.setTopicScope(false);
	std::cout << "changed +t -t" << std::endl;
}

void handleL(std::string const& option, Channel& channel, bool isPositive)
{
	if (isPositive)
	{
		if (option.empty())
			return;
		if (option.length() > 2)
		{
			std::cout << "Maximum user limit is 10" << std::endl;
			return;
		}
		int number = std::atoi(option.c_str());
		if (number < 1 || number > MAX_CLIENTS)
		{
			std::cout << "User limit must be between 1 and 10" << std::endl;
			return;
		}
		channel.setUserLimit(true, number);
	}
	else
		channel.setUserLimit(false, 0);
	std::cout << "changed +l -l" << std::endl;
}

void handleK(std::string const& option, Channel& channel, bool isPositive)
{
	if (isPositive)
	{
		if (option.empty())
			return;
		if (option.length() < 1 || option.length() > MAX_PASS_LEN)
		{
			std::cout << "Password must be between 1 and 15 characters" << std::endl;
			return;
		}
		channel.setPassword(true, option);
	}
	else
		channel.setPassword(false, NULL);
	std::cout << "changed +p -p" << std::endl;
}

void handleMode(std::istringstream& iss, Channel& channel, std::string word)
{
	std::string modes = "oitlk";
	void (*toHandle[5])(std::string const& option, Channel& channel, bool isPositive) = {handleO, handleI, handleT, handleL, handleK};

	bool isPositive = true;
	std::string option;
	iss >> option;
	std::cout << "option: " << option << std::endl;
	std::cout << "word: " << word << std::endl;
	for (int i = 0; word[i]; ++i)
	{
		if (word[i] == '+')
			isPositive = true;
		else if (word[i] == '-')
			isPositive = false;
		else
		{
			size_t index = modes.find(word[i], 0);
			std::cout << "index: " << index << std::endl;
			if (index != std::string::npos)
			{
				std::cout << RED "handle mode" RESET << std::endl;
				toHandle[index](option, channel, isPositive);
				if ((word[i] != 'i' && word[i] != 't') && ((word[i] != 'l' && word[i] != 'k') || isPositive))
					iss >> option;
			}
		}
	}
}

void mode(Client& client, Server& theServer, std::istringstream& iss)
{
	std::string word;
	iss >> word;
	word = word.substr(1);
	ChannelIterator channel = theServer.findChannel(word);
	if (channel != theServer.getChannels().end())
	{
		ClientIterator channelOperator = (*channel)->findOperator(client.getNickname());
		if (channelOperator != (*channel)->getOperators().end())
		{
			iss >> word;
			handleMode(iss, *(*channel), word);
		}
	}
}
