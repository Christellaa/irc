/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/05/20 11:36:36 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool checkModeK(std::string password, Channel& channel, Client& client)
{
    if (password.empty())
    {
        saveServerReply(client, ERR_BADCHANNELKEY(client.getNickname(), channel.getName(),
                                                  "No password given to join the channel"));
        return false;
    }
    else if (password != channel.getPassword())
    {
        saveServerReply(client, ERR_BADCHANNELKEY(client.getNickname(), channel.getName(),
                                                  "Wrong password given to join the channel"));
        return false;
    }
    return true;
}

bool checkModeL(Channel& channel, Client& client)
{
    if ((int)channel.getClients().size() < channel.getUserLimit() || channel.getUserLimit() == 0)
        return true;
    saveServerReply(client,
                    ERR_CHANNELISFULL(client.getNickname(), channel.getName(), "Channel is full"));
    return false;
}

bool checkModeI(Client& client, Channel& channel)
{
    if (client.findInvitedChannel(channel) != client.getInvitedChannels().end())
        return true;
    saveServerReply(client, ERR_INVITEONLYCHAN(client.getNickname(), channel.getName(),
                                               "Not invited to this channel"));
    return false;
}

bool checkModes(std::string password, Channel& channel, Client& client)
{
    bool modeOk = true;
    if (!channel.getPassword().empty())
        modeOk = checkModeK(password, channel, client);
    if (channel.getUserLimit() != 0)
        modeOk = checkModeL(channel, client);
    if (channel.isInviteOnly())
        modeOk = checkModeI(client, channel);
    return modeOk;
}

std::vector<std::string> splitString(std::string const& str, char delimiter)
{
    std::vector<std::string> res;
    std::string current;
    for (size_t i = 0; i < str.size(); ++i)
    {
        if (str[i] == delimiter)
        {
            res.push_back(current);
            current.clear();
        }
        else
            current += str[i];
    }
    res.push_back(current);
    return res;
}

void joinChannel(Channel& channel, Client* client, std::string const& password)
{
    if (checkModes(password, channel, *client))
    {
        channel.getClients().push_back(client);
        if (channel.isInviteOnly())
        {
            ChannelIterator channelToDel = client->findInvitedChannel(channel);
            client->getInvitedChannels().erase(channelToDel);
        }
        messageChannel(channel, JOIN(userPrefix(*client), channel.getName()));
        if (!channel.getTopicMessage().empty())
            saveServerReply(*client, RPL_TOPIC((*client).getNickname(), channel.getName(),
                                               " :" + channel.getTopicMessage()));
    }
}

void createAndJoinChannel(std::string const& channelName, Client* client, Server& theServer)
{
    if (channelName.length() > MAX_CHAR_CHANNEL)
        saveServerReply(*client,
                        ERR_BADCHANNAME(client->getNickname(), channelName,
                                        "Channel name must be maximum " +
                                            intToString(MAX_CHAR_CHANNEL) + " characters"));
    else if (theServer.getChannels().size() >= MAX_CHANNELS)
        saveServerReply(*client, ERR_TOOMANYCHANNELS(client->getNickname(), channelName,
                                                     "Too many channels in the server"));

    else
    {
        Channel *newChannel = new Channel(channelName);
        theServer.getChannels().push_back(newChannel);
        newChannel->getClients().push_back(client);
        newChannel->getOperators().push_back(client);
        saveServerReply(*client, JOIN(userPrefix(*client), newChannel->getName()));
    }
}

void join(Client *client, Server& theServer, std::istringstream& iss)
{
    std::string allChannels;
    std::string allPasswords;
    iss >> allChannels >> allPasswords;
    std::vector<std::string> channels = splitString(allChannels, ',');
    std::vector<std::string> passwords = splitString(allPasswords, ',');

    for (size_t i = 0; i < channels.size(); ++i)
    {
        std::string channelName = channels[i];
		if (channelName[0] != '#')
			channelName = "#" + channelName;
        std::string password = (i < passwords.size()) ? passwords[i] : "";
        if (hasForbiddenChars(channelName, "channel"))
        {
            saveServerReply(*client, ERR_BADCHANNAME((*client).getNickname(), channelName,
                                                     "Channel name has forbidden chars"));
            continue;
        }
        else
        {
            ChannelIterator hasChannel = theServer.findChannel(channelName);
            if (hasChannel != theServer.getChannels().end())
                joinChannel(*(*hasChannel), client, password);
            else
                createAndJoinChannel(channelName, client, theServer);
        }
    }
}
