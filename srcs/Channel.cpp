/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/04/29 11:36:06 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string const& name)
: _name(name), _topic(""), _inviteOnly(false), _topicScope(false),
_hasPassword(false), _password(""), _hasUserLimit(false), _userLimit(0) {}

Channel::~Channel() {}

std::string& Channel::getName() { return this->_name; }
ClientVec& Channel::getClients(void) { return this->_clients; }
ClientVec& Channel::getOperators(void) { return this->_operators; }

std::string Channel::getPassword(void)
{
	if (this->_hasPassword)
		return this->_password;
	return "";
}

double Channel::getUserLimit(void)
{
	if (this->_hasUserLimit)
		return this->_userLimit;
	return 0;
}

bool Channel::isInviteOnly(void) { return this->_inviteOnly;}

void Channel::giveOperatorRights(ClientIterator oldestClient)
{
	if (this->getOperators().size() != 0)
		return;
	this->getOperators().push_back(*oldestClient);
}
