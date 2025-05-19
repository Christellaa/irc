/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 14:35:54 by cde-sous          #+#    #+#             */
/*   Updated: 2025/05/19 13:39:13 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Macros.hpp"
#include "Server.hpp"

void invite(Client& client, Server& theServer, std::istringstream& iss)
{
    std::string target, channelName;
    iss >> target >> channelName;
    ChannelIterator channel = theServer.findChannel(channelName);
    if (channel == theServer.getChannels().end())
    {
        saveServerReply(
            client, ERR_NOSUCHCHANNEL(client.getNickname(), channelName, "Channel does not exist"));
        return;
    }
    if ((*channel)->findOperator(client.getNickname()) == (*channel)->getOperators().end())
    {
        saveServerReply(client, ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName,
                                                     client.getNickname() +
                                                         " is not an operator of " + channelName));
        return;
    }
    ClientIterator hasClient = (*channel)->findClient(target);
    if (hasClient != (*channel)->getClients().end())
    {
        saveServerReply(client, ERR_USERONCHANNEL(client.getNickname(), target, channelName,
                                                  target + " is already in the channel"));
        return;
    }
    ClientIterator clientTarget = theServer.findClientWithName(target);
    if (clientTarget == theServer.getClients().end())
    {
        saveServerReply(client, ERR_NOSUCHNICK(client.getNickname(), target,
                                               target + " does not exist in the server"));
        return;
    }
    if ((*clientTarget)->findInvitedChannel(*(*channel)) !=
        (*clientTarget)->getInvitedChannels().end())
    {
        saveServerReply(client, ERR_USERONCHANNEL(client.getNickname(), target, channelName,
                                                  target + " is already invited to the channel"));
        return;
    }
    (*clientTarget)->getInvitedChannels().push_back(*channel);
    saveServerReply(client, INVITE(userPrefix(client), channelName, target));
    saveServerReply(*(*clientTarget), INVITE(userPrefix(client), channelName, target));
}
