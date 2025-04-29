/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/04/29 15:20:32 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// function de pointer de function

void handleO(std::string& option)
{
	;
}

void handleMode(std::istringstream& iss, Channel& channel, std::string word)
{
	std::string modes = {"oitlk"};
	void (*toHandle[5])(std::string& option) = {handleO, handleI, handleT, handleL, handleK};

	bool isPositive = true;
	std::string option;
	iss >> option;
	for (int i = 0; word[i]; ++i)
	{
		if (word[i] == '+')
			isPositive = true;
		else if (word[i] == '-')
			isPositive = false;
		else if (std::strchr("oitlk", word[i]))
		{
			toHandle[word[i]](option);
			if (word[i] != 'i' && word[i] != 't')
				iss >> option;
		}
	}
}

void mode(Client* client, Server& theServer, std::istringstream& iss)
{
	/*
	- BOUCLE de chaque lettre de word
		- if letter is '+' => positif = true
		- if letter is '-' => positif = false
		- if letter is o, i, t, l or k
			- if it's i or t => don't go for info nor move to next info
			- if it's the others => go for info then move info to next
	*/
	std::string word;
	iss >> word; // = channel
	ChannelIterator it = theServer.getChannels().begin();
	ChannelIterator ite = theServer.getChannels().end();
	for (; it != ite; ++it)
	{
		if (word == (*it)->getName())
		{
			handleMode(iss, *(*it), word);
			return;
		}
	}
}
// MODE #a +ok-def client abc
