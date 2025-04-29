/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/04/29 12:28:46 by cde-sous         ###   ########.fr       */
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
		bool _inviteOnly;
		bool _topicScope;
		bool _hasPassword;
		std::string _password; // bloquer longueur
		bool _hasUserLimit;
		double _userLimit; // max TOTAL USER et min 1
		ClientVec _clients;
		ClientVec _operators;

	Channel();

	public:
		Channel(std::string const& name);
		~Channel();

		std::string& getName();
		ClientVec& getClients(void);
		ClientVec& getOperators(void);

		std::string getPassword(void);
		double getUserLimit(void);
		bool isInviteOnly(void);


		void giveOperatorRights(ClientIterator oldestClient);
};

#endif
