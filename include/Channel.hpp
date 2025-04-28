/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/04/28 12:09:16 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Macros.hpp"

class Client;

class Channel
{
	private:
		std::string _name;
		std::string _topic;
		bool _inviteOnly;
		bool _topicScope;
		bool _hasPassword;
		std::string _password;
		bool _hasUserLimit;
		double _userLimit;
		ClientVec _clients;
		ClientVec _operators;

	Channel();

	public:
		Channel(std::string const& name);
		~Channel();

		std::string& getName();
		ClientVec& getClients(void);
		ClientVec& getOperators(void);

		void giveOperatorRights(ClientIterator oldestClient);
};

#endif
