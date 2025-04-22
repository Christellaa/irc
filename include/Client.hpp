/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewu <jewu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:54:07 by jewu              #+#    #+#             */
/*   Updated: 2025/04/22 13:47:03 by jewu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Macros.hpp"

class Client
{
	private:
		int			_socketfd;
		std::string _nickname;
		std::string _username;
		std::string _password;
		std::string _msg;
		Client();

	public:
		Client(int socketfd);
		~Client();

		std::string getPassword(void);
		std::string getNickname(void);
		std::string getUsername(void);
		std::string& getMsg(void);
	
		void 		setPassword(std::string& password);
		void 		setNickname(std::string& nickname);
		void 		setUsername(std::string& username);
	
		static Client* findClient(std::vector<clientPair>& clients, int clientfd);
		void readClientMessage(int currentClientfd);
		void parseClientMessage(int currentClientfd);

		bool 		isWelcome;
};


#endif