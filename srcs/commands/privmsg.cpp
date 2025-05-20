/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:02:32 by jewu              #+#    #+#             */
/*   Updated: 2025/05/20 11:09:19 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void privmsgChannel(Server &theServer, Client &client, std::string const &channelName, std::string const &message)
{
    ChannelIterator channel = theServer.findChannel(channelName);
    if (channel != theServer.getChannels().end())
    {
        if ((*channel)->isInviteOnly() && (*channel)->findClient(client.getNickname()) == (*channel)->getClients().end())
        {
            saveServerReply(client, ERR_CANNOTSENDTOCHAN(client.getNickname(), (*channel)->getName(), "You cannot send message to invite only channel"));
            return;
        }
        else if (!(*channel)->getPassword().empty()  && (*channel)->findClient(client.getNickname()) == (*channel)->getClients().end())
        {
            saveServerReply(client, ERR_CANNOTSENDTOCHAN(client.getNickname(), (*channel)->getName(), "You cannot send message to channel needing a key"));
            return;
        }
        std::string command = message.substr(0, ' ');
        if (theServer.getBot().FindAndExecuteCommand(client, channelName, command, theServer))
            return;
        ClientIterator it = (*channel)->getClients().begin();
        ClientIterator ite = (*channel)->getClients().end();
        for (; it != ite; ++it)
        {
            if (client.getSocket() == (*it)->getSocket())
                continue;
            saveServerReply(*(*it), PRIVMSG(userPrefix(client), (*channel)->getName(), message));
        }
    }
    else
        saveServerReply(
            client, ERR_NOSUCHCHANNEL(client.getNickname(), channelName, "Channel does not exist"));
}

void privmsgClient(Server &theServer, Client &client, std::string const &target, std::string const &message)
{
    ClientIterator hasTarget = theServer.findClientWithName(target);
    if (hasTarget == theServer.getClients().end())
        saveServerReply(client, ERR_NOSUCHNICK(client.getNickname(), target, "Target not found"));
    else if ((*hasTarget)->getNickname() == client.getNickname())
        saveServerReply(client, ERR_ERRONEUSNICKNAME(client.getNickname(), target, "Target cannot be yourself"));
    else
        saveServerReply(*(*hasTarget), PRIVMSG(userPrefix(client), target, message));
}

void privmsg(Client &client, Server &theServer, std::istringstream &iss)
{
    std::string word;
    iss >> word;
    std::string message;
    std::getline(iss, message);
    int pos = message.find(':');
    message = message.substr(pos + 1);
    if (word[0] == '#')
        privmsgChannel(theServer, client, word, message);
    else
        privmsgClient(theServer, client, word, message);
}
