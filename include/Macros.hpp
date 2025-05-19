/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Macros.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:39:16 by jewu              #+#    #+#             */
/*   Updated: 2025/05/19 13:43:35 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACROS_HPP
#define MACROS_HPP

/****** CLASSES ******/

class Client;
class Server;
class Channel;
class Bot;

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
#include <map>
#include <queue>

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
#define MAX_CHANNELS 30

/****** TYPEDEFS ******/

typedef std::vector<Client*> ClientVec;
typedef std::vector<Channel*> ChannelVec;

typedef ClientVec::iterator ClientIterator;
typedef ChannelVec::iterator ChannelIterator;

typedef std::map<std::string, void (Bot::*)(Client&, std::string const&, Server&)>::iterator botIterator;

/****** COMMANDS ******/

void invite(Client& client, Server& theServer, std::istringstream& iss);
void join(Client *client, Server& theServer, std::istringstream& iss);
void kick(Client& client, Server& theServer, std::istringstream& iss);
void mode(Client& client, Server& theServer, std::istringstream& iss);
void nick(Client& client, Server& theServer, std::istringstream& iss);
void part(Client& client, Server& theServer, std::istringstream& iss);
void pong(Client& client);
void privmsg(Client& client, Server& theServer, std::istringstream& iss);
void quit(Client *client, Server& theServer);
void topic(Client& client, Server& theServer, std::istringstream& iss);

/****** FUNCTIONS ******/

void removeClientFromChannel(std::string const& clientNickname, Channel& channel, bool isKicked);
void removeOperator(std::string const& operatorNickname, Channel& channel);

void handle_signals(void);

void saveServerReply(Client& client, std::string const& reply);
void sendServerReply(Client& client);
void messageChannel(Channel& channel, std::string const& serverReply);

std::string welcomeClient(Client& client);
std::string userPrefix(Client& client);

std::string getIrcDate();
std::string intToString(int number);
std::string ft_tolower(std::string const& word);
bool        hasForbiddenChars(std::string const& name, std::string const& type);

/****** SIGNALS ******/

extern volatile int g_signal;

class SignalQuit : public std::exception
{
public:
    virtual const char *what() const throw();
};

/****** DEFINE COMMANDS ******/

#define RPL_WELCOME(nickname, prefix) \
    ":ircserv 001 " + nickname + " :Welcome to ircserv, " + prefix + "\r\n"
#define RPL_YOURHOST(nickname) \
    ":ircserv 002 " + nickname + " :Your host is ircserv, running version 1.0" + "\r\n"
#define RPL_CREATED(nickname, date) \
    ":ircserv 003 " + nickname + " :This server was created " + date + "\r\n"
#define RPL_SAVENICK(oldnickname, nickname) ":" + oldnickname + " NICK" + " :" + nickname + "\r\n"
#define RPL_NOTOPICSET(nickname, channelName, message) \
    ":ircserv 331 " + nickname + " " + channelName + message + "\r\n"
#define RPL_TOPIC(nickname, channelName, message) \
    ":ircserv 332 " + nickname + " " + channelName + message + "\r\n"
#define PRIVMSG(prefix, target, message) \
    ":" + prefix + " PRIVMSG " + target + " :" + message + "\r\n"
#define JOIN(prefix, channelName) ":" + prefix + " JOIN :" + channelName + "\r\n"
#define PART(prefix, channelName) ":" + prefix + " PART :" + channelName + "\r\n"
#define PONG(prefix) ":" + prefix + " PONG :ircserv" + "\r\n"
#define QUIT(prefix, clientSocket) \
    ":" + prefix + " QUIT :" + prefix + " on fd " + clientSocket + " left the server" + "\r\n"
#define MODE(prefix, channelName, mode, option) \
    ":" + prefix + " MODE " + channelName + " " + mode + " " + option + "\r\n"
#define KICK(prefix, channelName, target, message) \
    ":" + prefix + " KICK " + channelName + " " + target + " :" + message + "\r\n"
#define INVITE(prefix, channelName, target) \
    ":" + prefix + " INVITE " + target + " " + channelName + "\r\n"
#define TOPIC(prefix, channelName, message) \
    ":" + prefix + " TOPIC " + channelName + " :" + message + "\r\n"

/****** DEFINE ERRORS ******/

#define ERR_NOSUCHNICK(nickname, target, message) \
    ":ircserv 401 " + nickname + " " + target + " :" + message + "\r\n"
#define ERR_NOSUCHCHANNEL(nickname, channelName, message) \
    ":ircserv 403 " + nickname + " " + channelName + " :" + message + "\r\n"
#define ERR_CANNOTSENDTOCHAN(nickname, channelName, message) \
    ":ircserv 404 " + nickname + " " + channelName + " :" + message + "\r\n"
#define ERR_TOOMANYCHANNELS(nickname, channelName, message) \
    ":ircserv 405 " + nickname + " " + channelName + " :" + message + "\r\n"
#define ERR_INPUTTOOLONG(nickname, channelName, message) \
    ":ircserv 417 " + nickname + " " + channelName + " :" + message + "\r\n"
#define ERR_ERRONEUSNICKNAME(nickname, newNickname, message) \
    ":ircserv 432 " + nickname + " " + newNickname + " :" + message + "\r\n"
#define ERR_NICKNAMEINUSE(nickname, message) ":ircserv 433 * " + nickname + message + "\r\n"
#define ERR_USERNOTINCHANNEL(nickname, target, channelName, message) \
    ":ircserv 441 " + nickname + " " + target + " " + channelName + " :" + message + "\r\n"
#define ERR_NOTONCHANNEL(nickname, channelName, message) \
    ":ircserv 442 " + nickname + " " + channelName + message + "\r\n"
#define ERR_USERONCHANNEL(nickname, target, channelName, message) \
    ":ircserv 443 " + nickname + " " + target + " " + channelName + " :" + message + "\r\n"
#define ERR_NEEDMOREPARAMS(nickname, command, message) \
    ":ircserv 461 " + nickname + " " + command + " :" + message + "\r\n"
#define ERR_PASSWORD(nickname) ":ircserv 464 " + nickname + " :Password incorrect\r\n"
#define ERR_KEYSET(nickname, channelName, message) \
    ":ircserv 467 " + nickname + " " + channelName + " :" + message + "\r\n"
#define ERR_CHANNELISFULL(nickname, channelName, message) \
    ":ircserv 471 " + nickname + " " + channelName + " :" + message + "\r\n"
#define ERR_UNKNOWNMODE(nickname, mode, message) \
    ":ircserv 472 " + nickname + " " + mode + " :" + message + "\r\n"
#define ERR_INVITEONLYCHAN(nickname, channelName, message) \
    ":ircserv 473 " + nickname + " " + channelName + " :" + message + "\r\n"
#define ERR_BADCHANNELKEY(nickname, channelName, message) \
    ":ircserv 475 " + nickname + " " + channelName + " :" + message + "\r\n"
#define ERR_BADCHANNAME(nickname, channelName, message) \
    ":ircserv 479 " + nickname + " " + channelName + " :" + message + "\r\n"
#define ERR_CHANOPRIVSNEEDED(nickname, channelName, message) \
    ":ircserv 482 " + nickname + " " + channelName + " :" + message + "\r\n"
#define ERR_UMODEUNKNOWNFLAG(nickname, message) ":ircserv 501 " + nickname + " :" + message + "\r\n"
#define ERR_INVALIDMODEPARAM(nickname, channelName, mode, option, message)                        \
    ":ircserv 696 " + nickname + " " + channelName + " " + mode + " " + option + " :" + message + \
        "\r\n"

#endif
