/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/04/23 14:25:55 by codespace        ###   ########.fr       */
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
		std::vector<Client*> _clients;
		std::vector<Client*> _operators;

	Channel();

	public:
		Channel(std::string const& name);
		~Channel();

		std::string& getName();
		std::vector<Client*>& getClients(void);
		std::vector<Client*>& getOperators(void);
};

#endif
