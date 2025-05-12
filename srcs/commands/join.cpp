/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/05/12 16:47:07 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool checkModeK(std::string password, Channel& channel, Client& client)
{
    if (password.empty())
    {
        sendServerReply(client, ERR_BADCHANNELKEY(client.getNickname(), channel.getName(),
                                                  "No password given to join the channel"));
        return false;
    }
    else if (password != channel.getPassword())
    {
        sendServerReply(client, ERR_BADCHANNELKEY(client.getNickname(), channel.getName(),
                                                  "Wrong password given to join the channel"));
        return false;
    }
    return true;
}

bool checkModeL(Channel& channel, Client& client)
{
    if ((int) channel.getClients().size() < channel.getUserLimit() || channel.getUserLimit() == 0)
        return true;
    sendServerReply(client,
                    ERR_CHANNELISFULL(client.getNickname(), channel.getName(), "Channel is full"));
    return false;
}

bool checkModeI(Client& client, Channel& channel)
{
    if (client.findInvitedChannel(channel) != client.getInvitedChannels().end())
        return true;
    sendServerReply(client, ERR_INVITEONLYCHAN(client.getNickname(), channel.getName(),
                                               "Not invited to this channel"));
    return false;
}

bool checkModes(std::string password, Channel& channel, Client& client)
{
    bool modeOk = true;
    if (!channel.getPassword().empty())
        modeOk = checkModeK(password, channel, client);
    std::cout << "mode Ok key: " << modeOk << std::endl;
    if (channel.getUserLimit() != 0)
        modeOk = checkModeL(channel, client);
    std::cout << "mode Ok limit: " << modeOk << std::endl;
    if (channel.isInviteOnly())
        modeOk = checkModeI(client, channel);
    std::cout << "mode Ok invite: " << modeOk << std::endl;
    return modeOk;
}

void join(Client* client, Server& theServer, std::istringstream& iss)
{
    std::string channelName, password;
    iss >> channelName >> password;
    channelName             = channelName.substr(1);
    channelName             = ft_tolower(channelName);
    ChannelIterator channel = theServer.findChannel(channelName);
    if (channel != theServer.getChannels().end())
    {
        std::cout << "channelName: " << channelName << " + channel name: " << (*channel)->getName()
                  << std::endl;
        if (checkModes(password, *(*channel), *client))
        {
            (*channel)->getClients().push_back(client);
            if ((*channel)->isInviteOnly())
            {
                ChannelIterator channelToDel = client->findInvitedChannel(*(*channel));
                client->getInvitedChannels().erase(channelToDel);
            }
            std::cout << "Added " << client->getNickname() << " to channel "
                      << (*channel)->getName() << std::endl;
            sendServerReply(*client, JOIN(client->getNickname(), (*channel)->getName()));
        }
        return;
    }
    if (channelName.length() > MAX_CHAR_CHANNEL)
    {
        sendServerReply(*client,
                        ERR_BADCHANNAME(client->getNickname(), channelName,
                                        "Channel name must be maximum " +
                                            intToString(MAX_CHAR_CHANNEL) + " characters"));
        return;
    }
    if (theServer.getChannels().size() >= MAX_CHANNELS)
    {
        sendServerReply(*client, ERR_TOOMANYCHANNELS(client->getNickname(), channelName,
                                                     "Too many channels in the server"));
        return;
    }
    Channel* newChannel = new Channel(channelName);
    theServer.getChannels().push_back(newChannel);
    newChannel->getClients().push_back(client);
    newChannel->getOperators().push_back(client);
    std::cout << "Added " << client->getNickname() << " to NEW channel " << newChannel->getName()
              << std::endl;

    sendServerReply(*client, JOIN(client->getNickname(), newChannel->getName()));
}
