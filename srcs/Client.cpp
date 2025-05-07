/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:02:32 by jewu              #+#    #+#             */
/*   Updated: 2025/05/07 10:11:42 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

//####
//#Constructor & Destructor
//##################

Client::Client(int socketfd) : _socketfd(socketfd), isWelcome(true) {}

Client::~Client() {}

//####
//#Getters & Setters
//##################

std::string Client::getNickname(void)
{
    return this->_nickname;
}

std::string Client::getPassword(void)
{
    return this->_password;
}

std::string Client::getUsername(void)
{
    return this->_username;
}

std::string& Client::getMsg(void)
{
    return this->_msg;
}

int Client::getSocket(void)
{
    return this->_socketfd;
}

void Client::setPassword(std::string const& password)
{
    this->_password = password;
}
void Client::setNickname(std::string const& nickname)
{
    this->_nickname = nickname;
}
void Client::setUsername(std::string const& username)
{
    this->_username = username;
}

//####
//#Exceptions
//##################

//####
//#Functions
//##################

Channel* Client::findChannel(Channel& channel)
{
    ChannelIterator it  = this->_isInvited.begin();
    ChannelIterator ite = this->_isInvited.end();
    for (; it != ite; ++it)
    {
        if ((*it)->getName() == channel.getName())
            return *it;
    }
    return NULL;
}

void Client::parseWelcomeMessage(const std::string& line, Server& theServer)
{
    std::istringstream iss(line);
    std::string        word;
    iss >> word;

    if (word == "PASS")
    {
        iss >> word;
        this->setPassword(word);
    }
    else if (word == "NICK")
    {
        iss >> word;
        if (word.length() > 8)
            word = word.substr(0, 8);
        this->setNickname(word);
    }
    else if (word == "USER")
    {
        std::string user, unused1, unused2, realName;
        iss >> user >> unused1 >> unused2;
        std::getline(iss, realName);
        if (!realName.empty() && realName[0] == ':')
            realName = realName.substr(1);
        this->setUsername(realName);
    }

    if (!this->getPassword().empty() && !this->getNickname().empty() &&
        !this->getUsername().empty())
    {
        sameNickname(theServer);
        if (badPassword(theServer))
            return;

        std::string welcome_msg = welcome_client(this->getNickname(), this->getUsername());
        send(this->getSocket(), welcome_msg.c_str(), welcome_msg.length(), 0);
        this->isWelcome = false;
    }
}

bool Client::parseClientMessage(const std::string& line, Server& theServer)
{
    std::istringstream iss(line);
    std::string        word;
    iss >> word;

    if (word == "JOIN")
        join(this, theServer, iss);
    else if (word == "QUIT")
    {
        quit(this, theServer);
        return false;
    }
    else if (word == "MODE")
        mode(*this, theServer, iss);
    else if (word == "PING")
        pong(*this, iss);
    else if (word == "NICK")
        nick(*this, theServer, iss);
    else if (word == "PRIVMSG")
        privmsg(*this, theServer, iss);
    return true;
}

void Client::readClientMessage(Server& theServer)
{
    char    buffer[MAX_CHAR_MSG];
    ssize_t bytes = recv(this->getSocket(), buffer, sizeof(buffer), MSG_DONTWAIT);
    if (bytes == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return;
        else
            throw std::runtime_error("recv failed");
    }
    else if (bytes == 0)
    {
        std::cout << BOLD RED "Client " << this->getSocket() << " left the server" RESET
                  << std::endl;
        // quit(this, theServer);
        return;
    }

    _buffer.append(buffer, bytes);

    // 📦 Traiter chaque ligne complète
    size_t pos;
    std::cout << "buffer: " << _buffer << std::endl;
    while ((pos = _buffer.find("\n")) != std::string::npos)
    {
        std::string line = _buffer.substr(0, pos);
        _buffer.erase(0, pos + 1); // +1 pour supprimer le \n

        // Nettoyage du \r
        if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1, 1);

        std::cout << BOLD GREEN "Ligne reçue : [" RESET << line << "]" << std::endl;

        if (this->isWelcome)
            parseWelcomeMessage(line, theServer);
        else if (!parseClientMessage(line, theServer))
            return;
    }
}

// void Client::parseWelcomeMessage(Server& theServer)
//{
//	std::istringstream iss(this->getMsg());
//	std::string word;
//	while (iss >> word)
//	{
//		if (word == "PASS")
//		{
//			iss >> word;
//			this->setPassword(word);
//		}
//		else if (word == "NICK")
//		{
//			iss >> word;
//			if (word.length() > 8)
//				word = word.substr(0, 8);
//			this->setNickname(word);
//		}
//		else if (word == "USER")
//		{
//			std::string line;
//			std::getline(iss, line);
//			char pos = line.find(":");
//			line = line.substr(pos + 1, line.length());
//			this->setUsername(line);
//		}
//	}
//	if (this->isWelcome && !this->getPassword().empty() &&
//! this->getNickname().empty() && !this->getUsername().empty())
//	{
//		sameNickname(theServer);
//		if (badPassword(theServer))
//			return;
//		std::string welcome_msg = welcome_client(this->getNickname(),
// this->getUsername()); 		send(this->getSocket(),
// welcome_msg.c_str(), welcome_msg.length(), 0); this->isWelcome = false;
//	}
// }

// void Client::parseClientMessage(Server& theServer)
//{
//	if (this->isWelcome)
//	{
//		parseWelcomeMessage(theServer);
//		return;
//	}
//	std::istringstream iss(this->getMsg());
//	std::string word;
//	iss >> word;
//	if (word == "JOIN")
//		join(this, theServer, iss);
//	else if (word == "QUIT")
//	{
//		quit(this, theServer);
//		return;
//	}
//	else if (word == "MODE")
//		mode(theServer, iss);
//	else if (word == "PING")
//		pong(*this, iss);
//	else if (word == "NICK")
//		nick(*this, theServer, iss);
//	else if (word == "PRIVMSG")
//		privmsg(*this, theServer, iss);
//	this->getMsg().clear();
// }

// void Client::readClientMessage(Server& theServer)
//{
//	char buffer[MAX_CHAR_MSG];
//	while (1)
//	{
//		std::cout << BOLD PURPLE "Hey readClientMessage" RESET <<
// std::endl; 		std::cout << BOLD PURPLE "The message: " RESET <<
// this->getMsg() << std::endl; 		if (this->getMsg().find("\n") !=
// std::string::npos)
//		{
//			std::cout << BOLD RED "Found a \n" RESET << std::endl;
//			break;
//		}
//		ssize_t bytes = recv(this->getSocket(), buffer, sizeof(buffer),
// MSG_DONTWAIT); 		if (bytes == -1)
//		{
//			if (errno == EAGAIN || errno == EWOULDBLOCK)
//				return;
//			else
//				throw std::runtime_error("recv failed");
//		}
//		else if (bytes == 0)
//		{
//			std::cout << "Client left the server" << std::endl;
//			break;
//		}
//		else
//		{
//			this->getMsg().append(buffer, bytes);
//			std::cout << "Message: " << this->getMsg() << std::endl;
//		}
//	}
//	parseClientMessage(theServer);
// }

bool Client::badPassword(Server& theServer)
{
    if (this->getPassword() != theServer.getPassword())
    {
        ClientIterator client = theServer.findClient(this->getSocket());
        if (client != theServer.getClients().end())
        {
            const char* msg = "Connection refused: wrong password\n";
            send(this->getSocket(), msg, strlen(msg), 0);
            close(this->getSocket());
            theServer.getClients().erase(client);
            delete this;
            return true;
        }
    }
    return false;
}

void Client::sameNickname(Server& theServer)
{
    int         suffix   = 0;
    bool        conflict = true;
    std::string baseName = this->getNickname();
    std::string newName  = baseName;
    while (conflict)
    {
        conflict           = false;
        ClientIterator it  = theServer.getClients().begin();
        ClientIterator ite = theServer.getClients().end();
        for (; it != ite; ++it)
        {
            if (this != *it && (*it)->getNickname() == newName)
            {
                conflict = true;
                newName  = baseName + intToString(suffix++);
                break;
            }
        }
    }
    this->setNickname(newName);
}
