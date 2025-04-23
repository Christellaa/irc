/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/04/23 14:43:46 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void join(Client* client, Server& theServer, std::istringstream& iss)
{
	std::string word;
	iss >> word;
	word = word.substr(1);
	std::vector<Channel*>::iterator it = theServer.getChannels().begin();
	std::vector<Channel*>::iterator ite = theServer.getChannels().end();
	for (; it != ite; ++it)
	{
		if (word == (*it)->getName())
		{
			// check invite-only, etc
			(*it)->getClients().push_back(client);
			std::cout << "Added " << client->getNickname() << " to channel " << (*it)->getName() << std::endl;
			return;
		}
	}
	Channel newChannel(word);
	theServer.getChannels().push_back(&newChannel);
	newChannel.getClients().push_back(client);
	newChannel.getOperators().push_back(client);
	std::cout << "Added " << client->getNickname() << " to NEW channel " << newChannel.getName() << std::endl;
}
