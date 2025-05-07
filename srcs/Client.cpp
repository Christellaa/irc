/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewu <jewu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:02:32 by jewu              #+#    #+#             */
/*   Updated: 2025/05/07 15:39:14 by jewu             ###   ########.fr       */
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

std::string Client::getNickname(void){ return this->_nickname; }

std::string Client::getPassword(void){ return this->_password; }

std::string Client::getUsername(void){ return this->_username; }

std::string& Client::getMsg(void){ return this->_msg; }

int Client::getSocket(void){ return this->_socketfd; }

void Client::setPassword(std::string const& password){ this->_password = password; }

void Client::setNickname(std::string const& nickname){ this->_nickname = nickname; }

void Client::setUsername(std::string const& username){ this->_username = username; }

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

//void Client::parseWelcomeMessage(const std::string& line, Server& theServer)
//{
//    std::istringstream iss(line);
//    std::string        word;
//    iss >> word;

//    if (word == "PASS")
//    {
//        iss >> word;
//        this->setPassword(word);
//    }
//    else if (word == "NICK")
//    {
//        iss >> word;
//        if (word.length() > 8)
//            word = word.substr(0, 8);
//        this->setNickname(word);
//    }
//    else if (word == "USER")
//    {
//        std::string user, unused1, unused2, realName;
//        iss >> user >> unused1 >> unused2;
//        std::getline(iss, realName);
//        if (!realName.empty())
//            realName = realName.substr(2);
//        std::cout << "USERNAME: [" << realName << "]" << std::endl;
//        this->setUsername(realName);
//    }

//    if (!this->getPassword().empty() && !this->getNickname().empty() &&
//        !this->getUsername().empty())
//    {
//        sameNickname(theServer);
//        if (badPassword(theServer))
//            return;

//        std::string welcome_msg = WELCOME(this->getNickname());
//        // std::string welcome_msg = welcome_client(this->getNickname());
//        send(this->getSocket(), welcome_msg.c_str(), welcome_msg.length(), 0);
//        this->isWelcome = false;
//    }
//}

bool Client::parseWelcomeMessage(const std::string& line, Server& theServer)
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
        if (!realName.empty())
            realName = realName.substr(2);
        std::cout << "USERNAME: [" << realName << "]" << std::endl;
        this->setUsername(realName);
    }

    if (!this->getPassword().empty() && !this->getNickname().empty() &&
        !this->getUsername().empty())
    {
        sameNickname(theServer);
        if (badPassword(theServer))
            return false;
        std::string welcome_msg = WELCOME(this->getNickname());
        // std::string welcome_msg = welcome_client(this->getNickname());
        send(this->getSocket(), welcome_msg.c_str(), welcome_msg.length(), 0);
        this->isWelcome = false;
    }
	return true;
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
        return;
    }
    _buffer.append(buffer, bytes);
    size_t pos;
    std::cout << "buffer: " << _buffer << std::endl;
    while ((pos = _buffer.find("\n")) != std::string::npos)
    {
        std::string line = _buffer.substr(0, pos);
        _buffer.erase(0, pos + 1);
        if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1, 1);
        std::cout << BOLD GREEN "Ligne reçue : [" RESET << line << "]" << std::endl;
        if (this->isWelcome)
		{
            if (!parseWelcomeMessage(line, theServer))
				return;
		}
        else if (!parseClientMessage(line, theServer))
            return;
    }
}

bool Client::badPassword(Server& theServer)
{
    if (this->getPassword() != theServer.getPassword())
    {
        ClientIterator client = theServer.findClient(this->getSocket());
        if (client != theServer.getClients().end())
        {
			std::string msg = ERR_PASSWORD(this->getNickname());
            send(this->getSocket(), msg.c_str(), msg.size(), 0);
            close(this->getSocket());
			std::cout << BOLD RED "Connection failed: wrong password" RESET << std::endl;
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
