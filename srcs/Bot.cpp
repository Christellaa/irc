/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:29:24 by cde-sous          #+#    #+#             */
/*   Updated: 2025/05/14 15:20:13 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "Client.hpp"

Bot::Bot() : _name("Angry bird")
{
	this->_commands.push_back("!roll");
}

Bot::~Bot() {}

std::string const &Bot::getName()
{
	return this->_name;
}

bool Bot::findCommand(std::string const &command)
{
	if (command[0] == '!')
	{
		std::vector<std::string>::iterator hasCommand = std::find(this->_commands.begin(), this->_commands.end(), command);
		if (hasCommand != this->_commands.end())
			return true;
	}
	return false;
}

void Bot::handleRoll(Client &client, std::string const &channelName)
{
	std::string message = "5";
	sendServerReply(client, PRIVMSG(this->getName(), channelName, message));
}

void Bot::executeCommand(Client &client, std::string const &channelName, std::string const &command)
{
	std::string commands[1] = {"!roll"};
	void (*commandToExec[1])(Client &client, std::string const &channelName) = {Bot::handleRoll};
	size_t index = commands->find(command);
	if (index != std::string::npos)
		commandToExec[index](client, channelName);
}
