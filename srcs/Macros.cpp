/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Macros.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:38:01 by jewu              #+#    #+#             */
/*   Updated: 2025/05/09 14:26:55 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Macros.hpp"
#include "Client.hpp"

void sendServerReply(Client& client, std::string const& reply)
{
	send(client.getSocket(), reply.c_str(), reply.length(), 0);
	std::cout << reply << std::endl;
}

std::string welcomeClient(Client& client)
{
	std::string date = getIrcDate();
	return RPL_WELCOME(client.getNickname(), userPrefix(client)) + RPL_YOURHOST(client.getNickname()) + RPL_CREATED(client.getNickname(), date);
}

std::string userPrefix(Client& client)
{
	return client.getNickname() + "!" + client.getUsername() + "@localhost";
}

const char* SignalQuit::what() const throw(){
	return BOLD BLUE "\nQuitting server..." RESET;
}

std::string intToString(int number)
{
	std::ostringstream oss;
	oss << number;
	return oss.str();
}

#include <ctime>
#include <iomanip>

std::string getIrcDate()
{
	time_t now = time(NULL);
	struct tm* t = localtime(&now);

	static const char* days[]  = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
	static const char* months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
	                                "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	std::ostringstream oss;
	oss << days[t->tm_wday] << " "
	    << months[t->tm_mon] << " "
	    << std::setw(2) << std::setfill(' ') << t->tm_mday << " "
	    << std::setw(2) << std::setfill('0') << t->tm_hour << ":"
	    << std::setw(2) << std::setfill('0') << t->tm_min << ":"
	    << std::setw(2) << std::setfill('0') << t->tm_sec << " "
	    << (1900 + t->tm_year);

	return oss.str();
}
