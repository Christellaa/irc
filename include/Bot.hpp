/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:18:27 by cde-sous          #+#    #+#             */
/*   Updated: 2025/05/14 15:16:48 by cde-sous         ###   ########.fr       */
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
	std::vector<std::string> _commands;

public:
	Bot();
	~Bot();

	std::string const &getName();

	bool findCommand(std::string const &command);

	void executeCommand(Client &client, std::string const &channelName, std::string const &command);

	void handleRoll(Client &client, std::string const &channelName);
};

#endif
