/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewu <jewu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/05/06 12:14:39 by jewu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Macros.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Server
{
	private:
		int _port;
		int	_socketfd;
		int	_epoll_fd;

		std::string _password;
	
		ClientVec _clients;
	
		ChannelVec _channels; // max channels
	
		Server();

	public:
		Server(int port, std::string password);
		~Server();

		void launch_angrybots_server(void);
		void setting_server_socket(void);
		void setEpollfd(int fd);
		void addNewClient(int epoll_fd, struct epoll_event& ev);

		int getPort(void);
		int getSocket(void);
		int getEpollfd(void);
	
		std::string getPassword(void);

		ClientVec& getClients(void);
	
		ChannelVec& getChannels(void);

		Client* findClient(int clientfd);

		class InvalidSocket : public std::exception{
			public:
				virtual const char* what() const throw();
		};
		class SetsockoptFailure : public std::exception{
			public:
				virtual const char* what() const throw();
		};
		class BindingFailure : public std::exception{
			public:
				virtual const char* what() const throw();
		};
		class ListenFailure : public std::exception{
			public:
				virtual const char* what() const throw();
		};
};

#endif
