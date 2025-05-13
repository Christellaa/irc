/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:02:32 by jewu              #+#    #+#             */
/*   Updated: 2025/05/13 15:10:55 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void privmsg(Client& client, Server& theServer, std::istringstream& iss)
{
    std::string channelName;
    iss >> channelName;
    std::string message;
    std::getline(iss, message);
    int pos = message.find(':');
    message = message.substr(pos + 1);

    ChannelIterator channel = theServer.findChannel(channelName);
    if (channel != theServer.getChannels().end())
    {
        ClientIterator it  = (*channel)->getClients().begin();
        ClientIterator ite = (*channel)->getClients().end();
        for (; it != ite; ++it)
        {
            if (client.getSocket() == (*it)->getSocket())
                continue;
            sendServerReply(*(*it), PRIVMSG(client.getNickname(), (*channel)->getName(), message));
        }
    }
    else
        sendServerReply(
            client, ERR_NOSUCHCHANNEL(client.getNickname(), channelName, "Channel does not exist"));
}
