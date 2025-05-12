/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:58:18 by jewu              #+#    #+#             */
/*   Updated: 2025/05/12 13:17:39 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

void kick(Client& client, Server& theServer, std::istringstream& iss)
{
    std::string channelName, clientToKick;
    iss >> channelName >> clientToKick;
    channelName             = channelName.substr(1);
    ChannelIterator channel = theServer.findChannel(channelName);
    if (channel != theServer.getChannels().end())
    {
        ClientIterator chanop = (*channel)->findOperator(client.getNickname());
        if (chanop == (*channel)->getOperators().end())
        {
            sendServerReply(client,
                            ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName,
                                                 client.getNickname() +
                                                     " does not have the necessary privileges"));
            return;
        }
        ClientIterator hasClient = (*channel)->findClient(clientToKick);
        if (hasClient == (*channel)->getClients().end())
        {
            sendServerReply(client,
                            ERR_USERNOTINCHANNEL(client.getNickname(), clientToKick, channelName,
                                                 clientToKick + " is not in the channel"));
            return;
        }
        removeClientFromChannel(clientToKick, *(*channel));
        removeOperator(clientToKick, *(*channel));
        if ((*channel)->getClients().size() == 0)
        {
            delete *channel;
            channel = theServer.getChannels().erase(channel);
        }
        else if ((*channel)->getOperators().size() == 0)
            (*channel)->giveOperatorRights((*channel)->getClients().begin());
        std::string message;
        std::getline(iss, message);
        message = message.substr(2);
        sendServerReply(client, KICK(client.getNickname(), channelName, clientToKick, message));
    }
}
