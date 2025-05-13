/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewu <jewu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/05/13 13:59:10 by jewu             ###   ########.fr       */
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

std::string& Channel::getName()
{
    return this->_name;
}
ClientVec& Channel::getClients(void)
{
    return this->_clients;
}
ClientVec& Channel::getOperators(void)
{
    return this->_operators;
}

std::string Channel::getPassword(void)
{
    if (this->_hasPassword)
        return this->_password;
    return "";
}

std::string Channel::getTopicMessage(void){ return this->_topic; }

bool Channel::getTopicScope(void){ return this->_topicScope; }

int Channel::getUserLimit(void)
{
    if (this->_hasUserLimit)
        return this->_userLimit;
    return 0;
}

bool Channel::isInviteOnly(void)
{
    return this->_inviteOnly;
}

void Channel::setInviteOnly(bool value)
{
    this->_inviteOnly = value;
}

void Channel::setTopicScope(bool value)
{
    this->_topicScope = value;
}

void Channel::setTopicMessage(std::string message)
{
    this->_topic = message;
}

void Channel::setUserLimit(bool value, int newLimit)
{
    this->_hasUserLimit = value;
    this->_userLimit    = newLimit;
}

void Channel::setPassword(bool value, std::string const& password)
{
    this->_hasPassword = value;
    this->_password    = password;
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
