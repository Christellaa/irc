/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:54:07 by jewu              #+#    #+#             */
/*   Updated: 2025/04/28 15:18:52 by cde-sous         ###   ########.fr       */
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
		int getSocket(void);
	
		void 		setPassword(std::string const& password);
		void 		setNickname(std::string& nickname);
		void 		setUsername(std::string const& username);
	
		static Client* findClient(ClientVec& clients, int clientfd);
		void readClientMessage(Server& theServer);
		void parseClientMessage(Server& theServer);
		void parseWelcomeMessage(Server& theServer);

		bool badPassword(Server& theServer);
		void sameNickname(Server& theServer);

		bool 		isWelcome;
};


#endif
