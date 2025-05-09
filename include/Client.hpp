/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:54:07 by jewu              #+#    #+#             */
/*   Updated: 2025/05/09 13:57:52 by codespace        ###   ########.fr       */
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
    // quand clientA invite clientB, on va dans clientB et on rajoute le channel
    // de clientA dans le vector isInvited ici on check si on trouve le channel
    // que clientB tente de join dans son vector isInvited

    Client();

  public:
    Client(int socketfd);
    ~Client();

    std::string  getPassword(void);
    std::string  getNickname(void);
    std::string  getUsername(void);
    std::string& getMsg(void);

    int getSocket(void);

    void setPassword(std::string const& password);
    void setNickname(std::string const& nickname);
    void setUsername(std::string const& username);

    Channel* findInvitedChannel(Channel& channel);

    void readClientMessage(Server& theServer);
	  void sameNickname(Server& theServer);

    bool parseClientMessage(const std::string& line, Server& theServer);
	  bool parseWelcomeMessage(const std::string& line, Server& theServer);
    bool badPassword(Server& theServer);
    bool isWelcome;
};

#endif
