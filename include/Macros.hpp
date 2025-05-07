/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Macros.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewu <jewu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:39:16 by jewu              #+#    #+#             */
/*   Updated: 2025/05/07 17:10:56 by jewu             ###   ########.fr       */
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
void pong(Client& client, std::istringstream& iss);
void mode(Client& client, Server& theServer, std::istringstream& iss);
void quit(Client* client, Server& theServer);
void privmsg(Client& client, Server& theServer, std::istringstream& iss);

/****** FUNCTIONS ******/

std::string userPrefix(std::string nickname, std::string username);
// std::string welcome_client(std::string nickname, std::string prefix);
// std::string messageToSend(std::string const& prefix, std::string const& channelName, std::string const& message);

void	handle_signals(void);

std::string intToString(int number);

/****** SIGNALS ******/

extern int g_signal;

class SignalQuit : public std::exception{
	public:
		virtual const char* what() const throw();
};

#define WELCOME(nickname) ":ircserv 001 " + nickname + " :Welcome, " + nickname + "\r\n"
#define PRIVMSG(nickname, channelName, message)  ":" + nickname + " PRIVMSG #" + channelName + " :" + message + "\r\n"
#define JOIN(prefix, channelName) ":" + prefix + " JOIN :#" + channelName + "\r\n"
#define PONG(nickname, serverName) ":" + nickname + " PONG :" + serverName + "\r\n"
#define QUIT(prefix) ":" + prefix + " QUIT :leaving" + "\r\n"
#define MODE(nickname, channelName, modes, options) ":" + nickname + " MODE " + channelName + " " + modes + " " + options + "\r\n"

#define ERR_PASSWORD(nickname) ":ircserv 464 " + nickname + " :Password incorrect\r\n"

#endif
