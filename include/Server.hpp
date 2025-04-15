/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/04/15 15:07:32 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Macros.hpp"

class Server
{
	private:
		int _port;
		std::string _password;
		int	_socketfd;
	public:
		Server();
		Server(int port, std::string password);
		~Server();

		void launch_angrybots_server(void);
		void setting_server_socket(void);

		std::string getPassword(void);
		int getPort(void);

		class InvalidSocket : public std::exception{
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
