/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Macros.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:39:16 by jewu              #+#    #+#             */
/*   Updated: 2025/05/09 15:08:34 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACROS_HPP
#define MACROS_HPP

class Client;
class Server;
class Channel;

/****** LIBRARIES ******/

#include <iostream>
#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <limits>
#include <cerrno>
#include <sstream>
#include <netinet/in.h> //struct sockaddr_in
#include <sys/socket.h> //socket, setsockopt, getsockname...
#include <unistd.h> //close, lseek
#include <netdb.h> //getprotobyname, gethostbyname, getaddrinfo...
#include <arpa/inet.h> //htons, htonl, ntohs...
#include <csignal> //signal, sigaction
#include <fcntl.h> //fcntl
#include <poll.h> //poll
#include <sys/epoll.h> //epoll

/****** COLORS ******/

#define BLACK		"\033[0;30m"
#define	RED			"\033[0;31m"
#define	GREEN		"\033[0;32m"
#define	YELLOW		"\033[0;33m"
#define	BLUE		"\033[0;34m"
#define	PURPLE		"\033[0;35m"
#define	CYAN		"\033[0;36m"
#define	RESET		"\033[0m"
#define BORDEAUX	"\033[38;2;128;0;32m"
#define NEON_GREEN	"\033[38;2;57;255;20m"

/****** TEXT FORMATTING ******/

#define BOLD		"\033[1m"

/****** DEFINES ******/

#define INVALID_PORT		-1
#define INVALID_BIND		-1
#define INVALID_LISTEN		-1
#define EXIT_FAILURE		1
#define INVALID_SOCKET		0
#define EXIT_SUCCESS		0
#define FALSE				0
#define TRUE				1
#define MAX_CLIENTS			10
#define MAX_CHAR_MSG		512
#define MAX_CHAR_CHANNEL	50
#define MAX_PASS_LEN		15


/****** TYPEDEFS ******/

typedef std::vector<Client*> ClientVec;
typedef std::vector<Channel*> ChannelVec;

typedef ClientVec::iterator ClientIterator;
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

/****** FUNCTIONS ******/

void sendServerReply(Client& client, std::string const& reply);
std::string welcomeClient(Client& client);
std::string userPrefix(Client& client);
std::string intToString(int number);
std::string getIrcDate();


void removeClientFromChannel(std::string const& clientNickname, Channel& channel);
void removeOperator(std::string const& operatorNickname, Channel& channel);

void	handle_signals(void);

/****** SIGNALS ******/

extern int g_signal;

class SignalQuit : public std::exception{
	public:
		virtual const char* what() const throw();
};

/****** DEFINE COMMANDS ******/

#define RPL_WELCOME(nickname, prefix) ":ircserv 001 " + nickname + " :Welcome to ircserv, " + prefix + "\r\n"
#define RPL_YOURHOST(nickname) ":ircserv 002 " + nickname + " :Your host is ircserv, running version 1.0" + "\r\n"
#define RPL_CREATED(nickname, date) ":ircserv 003 " + nickname + " :This server was created " + date + "\r\n"
#define RPL_SAVENICK(oldnickname, nickname) ":" + oldnickname + " NICK" + " :" + nickname + "\r\n"
#define PRIVMSG(nickname, target, message) ":" + nickname + " PRIVMSG #" + target + " :" + message + "\r\n"
// if target is clientB and not channelA?
#define JOIN(nickname, channelName) ":" + nickname + " JOIN :#" + channelName + "\r\n"
#define PART(nickname, channelName) ":" + nickname + " PART :#" + channelName + "\r\n"
#define PONG(nickname) ":" + nickname + " PONG :ircserv" + "\r\n"
#define QUIT(nickname, clientSocket) ":" + nickname + " QUIT :" + nickname + " on fd " + clientSocket + " left the server" + "\r\n"
#define MODE(nickname, channelName, modes, options) ":" + nickname + " MODE " + channelName + " " + modes + " " + options + "\r\n"
#define KICK(nickname, channelName, target, message) ":" + nickname + " KICK #" + channelName + " " + target + " :" + message + "\r\n"
// KICK #test codespac0 :message de la mort
#define INVITE(nickname, channelName, target) ":" + nickname + " INVITE " + target + " #" + channelName + "\r\n"
// INVITE codespac0 #test
#define TOPIC(nickname, channelName, target, message) ":" + nickname + " TOPIC #" + channelName + " :" + message + "\r\n"
// TOPIC #test :lolololol

/****** DEFINE ERRORS ******/

#define ERR_PASSWORD(nickname) ":ircserv 464 " + nickname + " :Password incorrect\r\n"

#endif
