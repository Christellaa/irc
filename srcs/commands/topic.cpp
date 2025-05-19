/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/05/19 13:39:13 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Macros.hpp"
#include "Server.hpp"

static bool client_is_in_channel(Client &client, Channel &channel)
{
    ClientVec clientsToSearch = channel.getClients();
    for (ClientVec::iterator it = clientsToSearch.begin(); it != clientsToSearch.end(); ++it)
    {
        if (*it == &client)
            return true;
    }
    return false;
}

void topic(Client &client, Server &theServer, std::istringstream &iss)
{
    std::string channelName;
    iss >> channelName;
    ChannelIterator channel = theServer.findChannel(channelName);
    if (channel == theServer.getChannels().end())
    {
        saveServerReply(
            client, ERR_NOSUCHCHANNEL(client.getNickname(), channelName, " :Channel not found"));
        return;
    }
    if (!client_is_in_channel(client, *(*channel)))
    {
        saveServerReply(client, ERR_NOTONCHANNEL(client.getNickname(), (*channel)->getName(),
                                                 " :Client not in the channel"));
        return;
    }
    std::string message;
    std::getline(iss, message);
	if (!message.empty())
	    message = message.substr(2);
    if (message.empty())
    {
        if ((*channel)->getTopicMessage().empty())
            saveServerReply(client, RPL_NOTOPICSET(client.getNickname(), (*channel)->getName(),
                                                   " :No topic set"));
        else
            saveServerReply(client, RPL_TOPIC(client.getNickname(), (*channel)->getName(),
                                              " :" + (*channel)->getTopicMessage()));
        return;
    }
    ClientIterator chanop = (*channel)->findOperator(client.getNickname());
    if ((*channel)->getTopicScope() && chanop == (*channel)->getOperators().end())
    {
        saveServerReply(
            client,
            ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName,
                                 client.getNickname() + " does not have the necessary privileges"));
        return;
    }
    if (message.size() > 50)
    {
        saveServerReply(client, ERR_INPUTTOOLONG(client.getNickname(), channelName, "Input line was too long"));
        return;
    }
    (*channel)->setTopicMessage(message);
    messageChannel(*(*channel), TOPIC(userPrefix(client), channelName, message));
}
