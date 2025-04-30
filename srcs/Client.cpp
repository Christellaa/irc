/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:02:32 by jewu              #+#    #+#             */
/*   Updated: 2025/04/30 11:04:11 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

//####
//#Constructor & Destructor
//##################

Client::Client(int socketfd): _socketfd(socketfd), isWelcome(true) {}

Client::~Client(){}

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


void 		Client::setPassword(std::string const& password) { this->_password = password;}
void 		Client::setNickname(std::string const& nickname) { this->_nickname = nickname;}
void 		Client::setUsername(std::string const& username) { this->_username = username;}

//####
//#Exceptions
//##################

//####
//#Functions
//##################

Client* Client::findClient(ClientVec& clients, int clientfd)
{
	ClientIterator it = clients.begin();
	ClientIterator ite = clients.end();
	for (; it != ite; ++it)
	{
		if (clientfd == (*it)->getSocket())
			return *it;
	}
	return NULL;
}

Channel* Client::findChannel(Channel& channel)
{
	ChannelIterator it = this->_isInvited.begin();
	ChannelIterator ite = this->_isInvited.end();
	for (; it != ite; ++it)
	{
		if ((*it)->getName() == channel.getName())
			return *it;
	}
	return NULL;
}

void Client::readClientMessage(Server& theServer)
{
	char buffer[MAX_CHAR_MSG];
	while (1)
	{
		if (this->getMsg().find("\n") != std::string::npos)
			break;
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
			std::cout << "Client left the server" << std::endl;
			return;
		}
		else
		{
			this->getMsg().append(buffer, bytes);
			std::cout << "Message: " << this->getMsg() << std::endl;
		}
	}
	parseClientMessage(theServer);
}

void Client::parseClientMessage(Server& theServer)
{
	if (this->isWelcome)
	{
		parseWelcomeMessage(theServer);
		return;
	}
	std::istringstream iss(this->getMsg());
	std::string word;
	iss >> word;
	if (word == "JOIN")
		join(this, theServer, iss);
	else if (word == "QUIT")
	{
		quit(this, theServer);
		return;
	}
	else if (word == "MODE")
		mode(theServer, iss);
	this->getMsg().clear();
}

bool Client::badPassword(Server& theServer)
{
	if (this->getPassword() != theServer.getPassword())
	{
		ClientIterator it = theServer.getClients().begin();
		ClientIterator ite = theServer.getClients().end();
		for (; it != ite; ++it)
		{
			if (this->getNickname() == (*it)->getNickname())
			{
				const char* msg = "Connection refused: wrong password\n";
				send(this->getSocket(), msg, strlen(msg), 0);
				close(this->getSocket());
				theServer.getClients().erase(it);
				delete this;
				return true;
			}
		}
	}
	return false;
}

void Client::sameNickname(Server& theServer)
{
	int suffix = 0;
	bool conflict = true;
	std::string baseName = this->getNickname();
	std::string newName = baseName;
	while (conflict)
	{
		conflict = false;
		ClientIterator it = theServer.getClients().begin();
		ClientIterator ite = theServer.getClients().end();
		for (; it != ite; ++it)
		{
			if (this != *it && (*it)->getNickname() == newName)
			{
				conflict = true;
				newName = baseName + intToString(suffix++);
				break;
			}
		}
	}
	this->setNickname(newName);
}

void Client::parseWelcomeMessage(Server& theServer)
{
	std::istringstream iss(this->getMsg());
	std::string word;
	while (iss >> word)
	{
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
			std::string line;
			std::getline(iss, line);
			char pos = line.find(":");
			line = line.substr(pos + 1, line.length());
			this->setUsername(line);
		}
	}
	if (this->isWelcome && !this->getPassword().empty() && !this->getNickname().empty() && !this->getUsername().empty())
	{
		sameNickname(theServer);
		if (badPassword(theServer))
			return;
		std::string welcome_msg = welcome_client(this->getNickname(), this->getUsername());
		send(this->getSocket(), welcome_msg.c_str(), welcome_msg.length(), 0);
		this->isWelcome = false;
	}
}
