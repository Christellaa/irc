/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewu <jewu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:54:07 by jewu              #+#    #+#             */
/*   Updated: 2025/05/16 17:16:16 by jewu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Macros.hpp"

class Client
{
  private:
    int _socketfd;

    std::string _nickname;
    std::string _username;
    std::string _password;
    std::string _msg;
    std::string _buffer;

    ChannelVec _isInvited;
	std::vector<std::string> _messages;

    Client();

  public:
    Client(int socketfd);
    ~Client();

    int          getSocket(void);
    std::string& getNickname(void);
    std::string& getUsername(void);
    std::string& getPassword(void);
    std::string& getMsg(void);
    ChannelVec&     getInvitedChannels();
    std::vector<std::string>&     getMessages();

    void setNickname(std::string const& nickname);
    void setUsername(std::string const& username);
    void setPassword(std::string const& password);
    void addMessage(std::string const& message);

    void readClientMessage(Server& theServer);
    bool parseWelcomeMessage(const std::string& line, Server& theServer);
    bool parseClientMessage(const std::string& line, Server& theServer);

    bool badPassword(Server& theServer);
    void sameNickname(Server& theServer);

    ChannelIterator findInvitedChannel(Channel& channel);

    bool isWelcome;
};

#endif
