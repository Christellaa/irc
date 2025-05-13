/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewu <jewu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/05/13 13:35:35 by jewu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Macros.hpp"
#include "Server.hpp"

static bool client_is_in_channel(Client& client, Channel& channel)
{
	ClientVec clientsToSearch = channel.getClients();
	for (ClientVec::iterator it = clientsToSearch.begin(); it != clientsToSearch.end(); ++it)
	{
		if (*it == &client)
			return true;
	}
	return false;
}

void topic(Client& client, Server& theServer, std::istringstream& iss)
{
	std::string channelName;
	iss >> channelName;
	if (!channelName.empty())
		channelName = channelName.substr(1);
	else
		channelName = "";
	ChannelIterator channel = theServer.findChannel(channelName);
	if (channel == theServer.getChannels().end())
	{
		sendServerReply(client, ERR_NOSUCHCHANNEL(client.getNickname(), channelName, " :Channel not found"));
		return ;
	}
	if (!client_is_in_channel(client, *(*channel)))
	{
		sendServerReply(client, ERR_NOTONCHANNEL(client.getNickname(), (*channel)->getName(), " :Client not in the channel"));
		return ;
	}
	std::string message;
	std::getline(iss, message);
	message = message.substr(2);
	if (message.empty())
	{
		if ((*channel)->getTopicMessage().empty())
			sendServerReply(client, RPL_NOTOPICSET(client.getNickname(), (*channel)->getName(), " :No topic set"));
		else
			sendServerReply(client, RPL_TOPIC(client.getNickname(), (*channel)->getName(), " :" + (*channel)->getTopicMessage()));
		return ;
	}
	ClientIterator chanop = (*channel)->findOperator(client.getNickname());
    if ((*channel)->getTopicScope() && chanop == (*channel)->getOperators().end())
    {
        sendServerReply(client, ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName, client.getNickname() + " does not have the necessary privileges"));
        return;
    }
	(*channel)->setTopicMessage(message);
	ClientIterator it  = (*channel)->getClients().begin();
    ClientIterator ite = (*channel)->getClients().end();
	for (; it != ite; ++it)
    	sendServerReply(*(*it), TOPIC(client.getNickname(), channelName, clientToKick, message));
}