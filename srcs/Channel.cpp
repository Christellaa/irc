/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/05/15 15:01:28 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"

Channel::Channel(std::string const& name)
    : _name(name), _topic(""), _password(""), _hasPassword(false), _topicScope(false),
      _inviteOnly(false), _hasUserLimit(false), _userLimit(0)
{
}

Channel::~Channel() {}

std::string& Channel::getName(void) { return this->_name; }

std::string Channel::getTopicMessage(void) { return this->_topic; }

std::string Channel::getPassword(void)
{
    if (this->_hasPassword)
        return this->_password;
    return "";
}

bool Channel::getTopicScope(void) { return this->_topicScope; }

bool Channel::isInviteOnly(void) { return this->_inviteOnly; }

int Channel::getUserLimit(void)
{
    if (this->_hasUserLimit)
        return this->_userLimit;
    return 0;
}

ClientVec& Channel::getClients(void) { return this->_clients; }

ClientVec& Channel::getOperators(void) { return this->_operators; }

void Channel::setTopicMessage(std::string message) { this->_topic = message; }

void Channel::setPassword(bool value, std::string const& password)
{
    this->_hasPassword = value;
    this->_password    = password;
}

void Channel::setTopicScope(bool value) { this->_topicScope = value; }

void Channel::setInviteOnly(bool value) { this->_inviteOnly = value; }

void Channel::setUserLimit(bool value, int newLimit)
{
    this->_hasUserLimit = value;
    this->_userLimit    = newLimit;
}

void Channel::giveOperatorRights(ClientIterator oldestClient)
{
    if (!*oldestClient)
        return;
    ClientIterator channelOperator = this->findOperator((*oldestClient)->getNickname());
    if (channelOperator != this->getOperators().end())
        return;
    this->getOperators().push_back(*oldestClient);
}

void Channel::removeOperatorRights(Client& client)
{
    ClientIterator clientToRemove = this->findOperator(client.getNickname());
    if (clientToRemove != this->getOperators().end())
        this->getOperators().erase(clientToRemove);
}

ClientIterator Channel::findClient(std::string const& clientNickname)
{
    ClientIterator it  = this->getClients().begin();
    ClientIterator ite = this->getClients().end();
    for (; it != ite; ++it)
    {
        if (clientNickname == (*it)->getNickname())
            return it;
    }
    return ite;
}

ClientIterator Channel::findOperator(std::string const& operatorNickname)
{
    ClientIterator it  = this->getOperators().begin();
    ClientIterator ite = this->getOperators().end();
    for (; it != ite; ++it)
    {
        if (operatorNickname == (*it)->getNickname())
            return it;
    }
    return ite;
}

