/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewu <jewu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/05/06 12:19:57 by jewu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Macros.hpp"

class Client;

class Channel
{
	private:
		std::string _name; // bloquer longueur
		std::string _topic; // bloquer longueur
		std::string _password; // bloquer longueur
		bool _hasPassword;
		bool _topicScope;
		bool _inviteOnly;
		bool _hasUserLimit;
		int _userLimit;
		ClientVec _clients;
		ClientVec _operators;

		Channel();

	public:
		Channel(std::string const& name);
		~Channel();

		std::string& getName();
		std::string getPassword(void);

		ClientVec& getClients(void);
		ClientVec& getOperators(void);

		int getUserLimit(void);
	
		bool isInviteOnly(void);

		void setInviteOnly(bool value);
		void setTopicScope(bool value);
		void setUserLimit(bool value, int newLimit);
		void setPassword(bool value, std::string const& password);
		void giveOperatorRights(ClientIterator oldestClient);
		void removeOperatorRights(ClientIterator client);
};

#endif
