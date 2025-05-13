/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/05/13 15:10:49 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void part(Client& client, Server& theServer, std::istringstream& iss)
{
    std::string channelName;
    iss >> channelName;
    ChannelIterator channel = theServer.findChannel(channelName);
    if (channel != theServer.getChannels().end())
    {
        removeClientFromChannel(client.getNickname(), *(*channel));
        removeOperator(client.getNickname(), *(*channel));
        if ((*channel)->getClients().size() == 0)
        {
            delete *channel;
            channel = theServer.getChannels().erase(channel);
        }
        else if ((*channel)->getOperators().size() == 0)
            (*channel)->giveOperatorRights((*channel)->getClients().begin());

        ClientIterator it  = (*channel)->getClients().begin();
        ClientIterator ite = (*channel)->getClients().end();
        for (; it != ite; ++it)
            sendServerReply(*(*it), PART(client.getNickname(), channelName));
        sendServerReply(client, PART(client.getNickname(), channelName));
    }
}
