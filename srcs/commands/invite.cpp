/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 14:35:54 by cde-sous          #+#    #+#             */
/*   Updated: 2025/05/12 16:16:17 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Macros.hpp"
#include "Server.hpp"

void invite(Client& client, Server& theServer, std::istringstream& iss)
{
    std::string target, channelName;
    iss >> target >> channelName;
    channelName             = channelName.substr(1);
    ChannelIterator channel = theServer.findChannel(channelName);
    if (channel == theServer.getChannels().end())
    {
        sendServerReply(
            client, ERR_NOSUCHCHANNEL(client.getNickname(), channelName, "Channel does not exist"));
        return;
    }
    if ((*channel)->findOperator(client.getNickname()) == (*channel)->getOperators().end())
    {
        sendServerReply(client, ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName,
                                                     client.getNickname() +
                                                         " is not an operator of " + channelName));
        return;
    }
    ClientIterator hasClient = (*channel)->findClient(target);
    if (hasClient != (*channel)->getClients().end())
    {
        sendServerReply(client, ERR_USERONCHANNEL(client.getNickname(), target, channelName,
                                                  target + " is already in the channel"));
        return;
    }
    ClientIterator clientTarget = theServer.findClientWithName(target);
    if (clientTarget == theServer.getClients().end())
    {
        sendServerReply(client, ERR_NOSUCHNICK(client.getNickname(), target,
                                               target + " does not exist in the server"));
        return;
    }
    if ((*clientTarget)->findInvitedChannel(*(*channel)) !=
        (*clientTarget)->getInvitedChannels().end())
    {
        sendServerReply(client, ERR_USERONCHANNEL(client.getNickname(), target, channelName,
                                                  target + " is already invited to the channel"));
        return;
    }
    (*clientTarget)->getInvitedChannels().push_back(*channel);
    sendServerReply(client, INVITE(client.getNickname(), channelName, target));
}
