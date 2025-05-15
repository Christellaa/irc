/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:18:27 by cde-sous          #+#    #+#             */
/*   Updated: 2025/05/15 14:44:22 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include "Macros.hpp"
class Server;

class Bot
{
private:
	std::string _name;
	std::map<std::string, void (Bot::*)(Client &, std::string const&, Server &)> _commands;

public:
	Bot();
	~Bot();

	std::string const &getName();

	bool FindAndExecuteCommand(Client &client, std::string const& channelName, std::string const &command, Server &theServer);

	void handleHelp(Client &client, std::string const& channelName, Server &theServer);

	void handleBot(Client &client, std::string const& channelName, Server &theServer);

	void handleRoll(Client &client, std::string const& channelName, Server &theServer);

	void handleDate(Client &client, std::string const& channelName, Server &theServer);

	void handleNick(Client &client, std::string const& channelName, Server &theServer);
};

#endif
