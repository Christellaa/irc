/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/04/28 09:19:41 by cde-sous         ###   ########.fr       */
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
