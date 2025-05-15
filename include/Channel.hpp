/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/05/15 15:01:22 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Macros.hpp"

class Client;

class Channel
{
  private:
    std::string _name;
    std::string _topic;
    std::string _password;
    bool        _hasPassword;
    bool        _topicScope;
    bool        _inviteOnly;
    bool        _hasUserLimit;
    int         _userLimit;
    ClientVec   _clients;
    ClientVec   _operators;

    Channel();

  public:
    Channel(std::string const& name);
    ~Channel();

    std::string&   getName();
	  std::string    getTopicMessage(void);
    std::string    getPassword(void);
	  bool           getTopicScope(void);
    bool           isInviteOnly(void);
    int            getUserLimit(void);
    ClientVec&     getClients(void);
    ClientVec&     getOperators(void);

    void           setTopicMessage(std::string message);
    void           setPassword(bool value, std::string const& password);
    void           setTopicScope(bool value);
    void           setInviteOnly(bool value);
    void           setUserLimit(bool value, int newLimit);

    void           giveOperatorRights(ClientIterator oldestClient);
    void           removeOperatorRights(Client& client);

    ClientIterator findClient(std::string const& clientNickname);
    ClientIterator findOperator(std::string const& operatorNickname);
};

#endif
