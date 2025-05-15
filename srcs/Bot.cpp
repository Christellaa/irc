/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:29:24 by cde-sous          #+#    #+#             */
/*   Updated: 2025/05/15 11:59:42 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "Client.hpp"

Bot::Bot() : _name("AngryBird")
{
	this->_commands.insert(std::make_pair("!help", &Bot::handleHelp));
	this->_commands.insert(std::make_pair("!bot", &Bot::handleBot));
	this->_commands.insert(std::make_pair("!roll", &Bot::handleRoll));
	this->_commands.insert(std::make_pair("!date", &Bot::handleDate));
}

Bot::~Bot() {}

std::string const &Bot::getName()
{
	return this->_name;
}

void Bot::handleHelp(Client &client, std::string const &channelName)
{
	std::vector<std::string> roasts;
	roasts.push_back("Help? Did you forget how to use a search engine, or is your brain just on permanent vacation?");
	roasts.push_back("Help? Sure, let me just drop everything and babysit you through the basics.");
	roasts.push_back("I'm pretty sure even Google is tired of your questions, whatever.");
	roasts.push_back("You are so useless🤣");
	std::srand(time(0));
	std::string message = roasts[std::rand() % 4];
	sendServerReply(client, PRIVMSG(this->getName(), channelName, BOLD RED + "🤬📢 " + message + RESET));
	botIterator it = this->_commands.begin();
	botIterator ite = this->_commands.end();
	for (; it != ite; ++it)
		sendServerReply(client, PRIVMSG(this->getName(), channelName, GREEN + (*it).first + RESET));
}

void Bot::handleBot(Client &client, std::string const &channelName)
{
	sendServerReply(client, PRIVMSG(this->getName(), channelName, BOLD RED + "HI!!!🐦 I'm AngryBird! I'm not angry! Don't push me!🤬" + RESET));
}

void Bot::handleRoll(Client &client, std::string const &channelName)
{
	std::vector<std::string> roasts;
	roasts.push_back("Dice roll done — hope it's luckier than your last idea.");
	roasts.push_back("Your luck's worse than the Squid Game contestants who never even got to the final round.");
	roasts.push_back("The only thing 'rolling' here is your last shred of credibility.");
	roasts.push_back("JK might be Rowling in cash, but you're just rolling in shame.");
	roasts.push_back("How much I like you out of 50 (spoiler: not much🤪)");
	roasts.push_back("Rolling the dice for you, because watching you try would be painful.");
	std::srand(time(0));
	std::string message = roasts[std::rand() % 6];
	int randomNumber = std::rand() % 6 + 1;
	std::stringstream ss;
	ss << randomNumber;
	sendServerReply(client, PRIVMSG(this->getName(), channelName, BOLD RED + message + RESET + GREEN + "🎲 " + ss.str() + RESET));
}

void Bot::handleDate(Client &client, std::string const &channelName)
{
	std::vector<std::string> roasts;
	roasts.push_back(BOLD RED "Today is the day to wonder how you've survived this long without basic info." RESET);
	roasts.push_back(BOLD RED "Today is the day to teach you how to check the date by yourself like a big kid." RESET);
	roasts.push_back(BOLD RED "Today is the day to shut off your brain for my own sanity." RESET);
	roasts.push_back(BOLD RED "Today is the day to embrace the fact that asking a bot for the date is peak 2025 energy." RESET);

	std::srand(time(0));
	std::string message = roasts[std::rand() % 4];
	std::string date = getIrcDate();
	sendServerReply(client, PRIVMSG(this->getName(), channelName, message + GREEN + " 📅 Here for the idiot: " + date + RESET));
}

bool Bot::FindAndExecuteCommand(Client &client, std::string const &channelName, std::string const &command)
{
	botIterator it = this->_commands.find(command);
	if (it != this->_commands.end())
	{
		(this->*(*it).second)(client, channelName);
		return true;
	}
	return false;
}

// change nick
