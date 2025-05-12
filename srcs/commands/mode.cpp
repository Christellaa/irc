/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/05/12 12:54:31 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool handleO(std::string const& option, Channel& channel, bool isPositive, Client& client)
{
    if (option.empty())
    {
        sendServerReply(client,
                        ERR_NEEDMOREPARAMS(client.getNickname(), "MODE",
                                           "No client given to give or remove operator rights"));
        return false;
    }
    if (isPositive)
    {
        ClientIterator clientIt = channel.findClient(option);
        if (clientIt != channel.getClients().end())
        {
            channel.giveOperatorRights(clientIt);
            return true;
        }
        sendServerReply(
            client, ERR_USERNOTINCHANNEL(client.getNickname(), option, channel.getName(),
                                         std::string(option) + " is not in #" + channel.getName()));
    }
    else
    {
        ClientIterator channelOperator = channel.findOperator(option);
        if (channelOperator != channel.getOperators().end())
        {
            channel.removeOperatorRights(*(*channelOperator));
            return true;
        }
    }
    return false;
}

bool handleI(std::string const& option, Channel& channel, bool isPositive, Client& client)
{
    (void) option;
    (void) client;
    if (isPositive)
        channel.setInviteOnly(true);
    else
        channel.setInviteOnly(false);
    std::cout << "changed +i -i" << std::endl;
    return true;
}

bool handleT(std::string const& option, Channel& channel, bool isPositive, Client& client)
{
    (void) option;
    (void) client;
    if (isPositive)
        channel.setTopicScope(true);
    else
        channel.setTopicScope(false);
    std::cout << "changed +t -t" << std::endl;
    return true;
}

bool handleL(std::string const& option, Channel& channel, bool isPositive, Client& client)
{
    if (isPositive)
    {
        if (option.empty())
        {
            sendServerReply(client,
                            ERR_NEEDMOREPARAMS(client.getNickname(), "MODE", "No limit given"));
            return false;
        }
        if (option.length() > 2)
        {
            sendServerReply(client, ERR_INVALIDMODEPARAM(client.getNickname(), channel.getName(),
                                                         "+l", option, "Maximum user limit is 10"));
            return false;
        }
        int number = std::atoi(option.c_str());
        if (number < 1 || number > MAX_CLIENTS)
        {
            sendServerReply(client,
                            ERR_INVALIDMODEPARAM(client.getNickname(), channel.getName(), "+l",
                                                 option, "User limit must be between 1 and 10"));
            return false;
        }
        channel.setUserLimit(true, number);
    }
    else
        channel.setUserLimit(false, 0);
    return true;
}

bool handleK(std::string const& option, Channel& channel, bool isPositive, Client& client)
{
    if (isPositive)
    {
        if (option.empty())
        {
            sendServerReply(client,
                            ERR_NEEDMOREPARAMS(client.getNickname(), "MODE", "No password given"));
            return false;
        }
        else if (option.length() < 1 || option.length() > MAX_PASS_LEN)
        {
            sendServerReply(client, ERR_KEYSET(client.getNickname(), channel.getName(),
                                               "Password must be between 1 and 15 characters"));
            return false;
        }
        channel.setPassword(true, option);
    }
    else
        channel.setPassword(false, "");
    return true;
}

void handleModes(std::istringstream& iss, Channel& channel, std::string modes, Client& client)
{
    std::string modesAllowed            = "oitlk";
    bool (*toHandle[5])(std::string const& option, Channel& channel, bool isPositive,
                        Client& client) = {handleO, handleI, handleT, handleL, handleK};

    bool        isPositive = true;
    std::string option;
    iss >> option;
    std::cout << "modes: " << modes << std::endl;
    std::cout << "option: " << option << std::endl;
    bool modeOk = true;
    for (int i = 0; modes[i]; ++i)
    {
        if (modes[i] == '+')
            isPositive = true;
        else if (modes[i] == '-')
            isPositive = false;
        else
        {
            std::string modeToSend = (isPositive ? "+" : "-") + std::string(1, modes[i]);
            size_t      index      = modesAllowed.find(modes[i], 0);
            if (index != std::string::npos)
            {
                modeOk = toHandle[index](option, channel, isPositive, client);
                if (modeOk)
                    sendServerReply(
                        client, MODE(client.getNickname(), channel.getName(), modeToSend, option));
                if ((modes[i] != 'i' && modes[i] != 't') &&
                    ((modes[i] != 'l' && modes[i] != 'k') || isPositive))
                    iss >> option;
            }
            sendServerReply(client,
                            ERR_UNKNOWNMODE(client.getNickname(), modeToSend, "Unknown mode"));
        }
    }
}

void mode(Client& client, Server& theServer, std::istringstream& iss)
{
    std::string channelName, modes;
    iss >> channelName >> modes;
    channelName             = channelName.substr(1);
    ChannelIterator channel = theServer.findChannel(channelName);
    if (channel != theServer.getChannels().end())
    {
        ClientIterator channelOperator = (*channel)->findOperator(client.getNickname());
        if (channelOperator != (*channel)->getOperators().end())
            handleModes(iss, *(*channel), modes, client);
        else
            sendServerReply(client, ERR_CHANOPRIVSNEEDED(
                                        client.getNickname(), channelName,
                                        client.getNickname() +
                                            " does not have the necessary privileges for MODE"));
    }
    else
        sendServerReply(client, ERR_UMODEUNKNOWNFLAG(client.getNickname(),
                                                     "MODE can only affect an existing channel"));
}
