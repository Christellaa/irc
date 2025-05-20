/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Macros.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:38:01 by jewu              #+#    #+#             */
/*   Updated: 2025/05/20 10:18:23 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Macros.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void saveServerReply(Client& client, std::string const& reply)
{
    client.addMessage(reply);
}

void sendServerReply(Client& client)
{
    if (client.getMessages().size() != 0)
    {
        std::string reply = client.getMessages().front();
        send(client.getSocket(), reply.c_str(), reply.length(), 0);
        std::cout << BOLD BLUE "SENDING" RESET << " to client [" << client.getNickname() << "] the reply:" << std::endl;
        std::cout << reply << std::endl;
        client.getMessages().pop();
    }
}

void messageChannel(Channel& channel, std::string const& serverReply)
{
    ClientIterator it = channel.getClients().begin();
    ClientIterator ite = channel.getClients().end();
    for (; it != ite; ++it)
        saveServerReply(*(*it), serverReply);
}

std::string welcomeClient(Client& client)
{
    std::string date = getIrcDate();
    return RPL_WELCOME(client.getNickname(), userPrefix(client)) +
           RPL_YOURHOST(client.getNickname()) + RPL_CREATED(client.getNickname(), date);
}

std::string userPrefix(Client& client)
{
    return client.getNickname() + "!" + client.getUsername() + "@localhost";
}

std::string getIrcDate()
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    static const char *days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    static const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                   "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    std::ostringstream oss;
    oss << days[t->tm_wday] << " " << months[t->tm_mon] << " " << std::setw(2) << std::setfill(' ')
        << t->tm_mday << " " << std::setw(2) << std::setfill('0') << t->tm_hour << ":"
        << std::setw(2) << std::setfill('0') << t->tm_min << ":" << std::setw(2)
        << std::setfill('0') << t->tm_sec << " " << (1900 + t->tm_year);

    return oss.str();
}

std::string intToString(int number)
{
    std::ostringstream oss;
    oss << number;
    return oss.str();
}

std::string ft_tolower(std::string const& word)
{
    std::string result = word;
    std::transform(result.begin(), result.end(), result.begin(),
                   static_cast<int (*)(int)>(std::tolower));
    return result;
}

bool hasForbiddenChars(std::string const& name, std::string const& type)
{
    if (type == "channel")
    {
        size_t hasForbidden = name.find('\a');
        if (hasForbidden != std::string::npos)
            return true;
    }
    else if (type == "client")
    {
        if (name[0] == '#')
            return true;
    }
    return false;
}

const char *SignalQuit::what() const throw()
{
    return BOLD BLUE "\nQuitting server..." RESET;
}
