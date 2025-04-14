/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Macros.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewu <jewu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:39:16 by jewu              #+#    #+#             */
/*   Updated: 2025/04/14 12:18:26 by jewu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACROS_HPP
#define MACROS_HPP

/****** LIBRARIES ******/

#include <iostream>
#include <cstdlib>
#include <exception>
#include <map>
#include <string>
#include <limits>
#include <sys/socket.h> //socket, setsockopt, getsockname...
#include <unistd.h> //close, lseek
#include <netdb.h> //getprotobyname, gethostbyname, getaddrinfo...
#include <arpa/inet.h> //htons, htonl, ntohs...
#include <signal.h> //signal, sigaction
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

/****** DEFINES ******/

#define INVALID_FD 		-1
#define INVALID_PORT 	-1
#define EXIT_FAILURE	1
#define EXIT_SUCCESS	0
#define FALSE			0
#define TRUE			1

/****** ENUM ******/

#endif