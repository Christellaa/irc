/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Macros.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewu <jewu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:39:16 by jewu              #+#    #+#             */
/*   Updated: 2025/05/13 13:41:27 by jewu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACROS_HPP
#define MACROS_HPP

class Client;
class Server;
class Channel;

/****** LIBRARIES ******/

#include <algorithm>
#include <arpa/inet.h> //htons, htonl, ntohs...
#include <cerrno>
#include <csignal> //signal, sigaction
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <exception>
#include <fcntl.h> //fcntl
#include <iomanip>
#include <iostream>
#include <limits>
#include <netdb.h>      //getprotobyname, gethostbyname, getaddrinfo...
#include <netinet/in.h> //struct sockaddr_in
#include <poll.h>       //poll
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/epoll.h>  //epoll
#include <sys/socket.h> //socket, setsockopt, getsockname...
#include <unistd.h>     //close, lseek
#include <vector>

/****** COLORS ******/

#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"
#define BORDEAUX "\033[38;2;128;0;32m"
#define NEON_GREEN "\033[38;2;57;255;20m"

/****** TEXT FORMATTING ******/

#define BOLD "\033[1m"

/****** DEFINES ******/

#define INVALID_PORT -1
#define INVALID_BIND -1
#define INVALID_LISTEN -1
#define EXIT_FAILURE 1
#define INVALID_SOCKET 0
#define EXIT_SUCCESS 0
#define FALSE 0
#define TRUE 1
#define MAX_CLIENTS 10
#define MAX_CHAR_MSG 512
#define MAX_CHAR_CHANNEL 50
#define MAX_PASS_LEN 15
#define MAX_CHANNELS 3

/****** TYPEDEFS ******/

typedef std::vector<Client*>  ClientVec;
typedef std::vector<Channel*> ChannelVec;

typedef ClientVec::iterator  ClientIterator;
typedef ChannelVec::iterator ChannelIterator;

/****** UTIL FUNCTIONS ******/

void join(Client* client, Server& theServer, std::istringstream& iss);
void nick(Client& client, Server& theServer, std::istringstream& iss);
void pong(Client& client);
void mode(Client& client, Server& theServer, std::istringstream& iss);
void quit(Client* client, Server& theServer);
void privmsg(Client& client, Server& theServer, std::istringstream& iss);
void part(Client& client, Server& theServer, std::istringstream& iss);
void kick(Client& client, Server& theServer, std::istringstream& iss);
void invite(Client& client, Server& theServer, std::istringstream& iss);
void topic(Client& client, Server& theServer, std::istringstream& iss);

/****** FUNCTIONS ******/

void        sendServerReply(Client& client, std::string const& reply);
std::string welcomeClient(Client& client);
std::string userPrefix(Client& client);
std::string intToString(int number);
std::string getIrcDate();
std::string ft_tolower(std::string const& word);

void removeClientFromChannel(std::string const& clientNickname, Channel& channel);
void removeOperator(std::string const& operatorNickname, Channel& channel);

void handle_signals(void);

/****** SIGNALS ******/

extern int g_signal;

class SignalQuit : public std::exception
{
  public:
    virtual const char* what() const throw();
};

/****** DEFINE COMMANDS ******/

#define RPL_WELCOME(nickname, prefix)                                                              \
    ":ircserv 001 " + nickname + " :Welcome to ircserv, " + prefix + "\r\n"
#define RPL_YOURHOST(nickname)                                                                     \
    ":ircserv 002 " + nickname + " :Your host is ircserv, running version 1.0" + "\r\n"
#define RPL_CREATED(nickname, date)                                                                \
    ":ircserv 003 " + nickname + " :This server was created " + date + "\r\n"
#define RPL_SAVENICK(oldnickname, nickname) ":" + oldnickname + " NICK" + " :" + nickname + "\r\n"
#define RPL_NOTOPICSET(nickname, channelName, message) ":ircserv 331 " +  nickname + " #" + channelName + message + "\r\n"
#define RPL_TOPIC(nickname, channelName, message) ":ircserv 332 " + nickname + " #" + channelName + message + "\r\n"

#define PRIVMSG(nickname, target, message)                                                         \
    ":" + nickname + " PRIVMSG #" + target + " :" + message + "\r\n"
// if target is clientB and not channelA?
#define JOIN(nickname, channelName) ":" + nickname + " JOIN :#" + channelName + "\r\n"
#define PART(nickname, channelName) ":" + nickname + " PART :#" + channelName + "\r\n"
#define PONG(nickname) ":" + nickname + " PONG :ircserv" + "\r\n"
#define QUIT(nickname, clientSocket)                                                               \
    ":" + nickname + " QUIT :" + nickname + " on fd " + clientSocket + " left the server" + "\r\n"
#define MODE(nickname, channelName, mode, option)                                                  \
    ":" + nickname + " MODE #" + channelName + " " + mode + " " + option + "\r\n"
#define KICK(nickname, channelName, target, message)                                               \
    ":" + nickname + " KICK #" + channelName + " " + target + " :" + message + "\r\n"
#define INVITE(nickname, channelName, target)                                                      \
    ":" + nickname + " INVITE " + target + " #" + channelName + "\r\n"
#define TOPIC(nickname, channelName, message)                                              \
    ":" + nickname + " TOPIC #" + channelName + " :" + message + "\r\n"

/****** DEFINE ERRORS ******/

#define ERR_NOSUCHNICK(nickname, target, message)                                                  \
    ":ircserv 401 " + nickname + " " + target + " :" + message + "\r\n"
#define ERR_NOSUCHCHANNEL(nickname, channelName, message)                                          \
    ":ircserv 403 " + nickname + " #" + channelName + " :" + message + "\r\n"
#define ERR_TOOMANYCHANNELS(nickname, channelName, message)                                        \
    ":ircserv 405 " + nickname + " #" + channelName + " :" + message + "\r\n"
#define ERR_NICKNAMEINUSE(nickname, message) ":ircserv 433 * " + nickname + message + "\r\n"
#define ERR_USERNOTINCHANNEL(nickname, target, channelName, message)                               \
    ":ircserv 441 " + nickname + " " + target + " #" + channelName + " :" + message + "\r\n"
#define ERR_NOTONCHANNEL(nickname, channelName, message) ":ircserv 442 " + nickname + " #" + channelName + message + "\r\n"
#define ERR_USERONCHANNEL(nickname, target, channelName, message)                                  \
    ":ircserv 443 " + nickname + " " + target + " " + channelName + " :" + message + "\r\n"
#define ERR_NEEDMOREPARAMS(nickname, command, message)                                             \
    ":ircserv 461 " + nickname + " " + command + " :" + message + "\r\n"
#define ERR_PASSWORD(nickname) ":ircserv 464 " + nickname + " :Password incorrect\r\n"
#define ERR_KEYSET(nickname, channelName, message)                                                 \
    ":ircserv 467 " + nickname + " #" + channelName + " :" + message + "\r\n"
#define ERR_CHANNELISFULL(nickname, channelName, message)                                          \
    ":ircserv 471 " + nickname + " #" + channelName + " :" + message + "\r\n"
#define ERR_UNKNOWNMODE(nickname, mode, message)                                                   \
    ":ircserv 472 " + nickname + " " + mode + " :" + message + "\r\n"
#define ERR_INVITEONLYCHAN(nickname, channelName, message)                                         \
    ":ircserv 473 " + nickname + " #" + channelName + " :" + message + "\r\n"
#define ERR_BADCHANNELKEY(nickname, channelName, message)                                          \
    ":ircserv 475 " + nickname + " #" + channelName + " :" + message + "\r\n"
#define ERR_BADCHANNAME(nickname, channelName, message)                                            \
    ":ircserv 479 " + nickname + " #" + channelName + " :" + message + "\r\n"
#define ERR_CHANOPRIVSNEEDED(nickname, channelName, message)                                       \
    ":ircserv 482 " + nickname + " #" + channelName + " :" + message + "\r\n"
#define ERR_UMODEUNKNOWNFLAG(nickname, message) ":ircserv 501 " + nickname + " :" + message + "\r\n"
#define ERR_INVALIDMODEPARAM(nickname, channelName, mode, option, message)                         \
    ":ircserv 696 " + nickname + " #" + channelName + " " + mode + " " + option + " :" + message + \
        "\r\n"

#endif
