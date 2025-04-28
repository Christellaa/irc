/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:02:32 by jewu              #+#    #+#             */
/*   Updated: 2025/04/28 09:33:38 by cde-sous         ###   ########.fr       */
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
void 		Client::setNickname(std::string const& nickname)
{
	// if word > 9 -> trunc
	this->_nickname = nickname;
}

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
			// cleanup + exit
			std::cout << "Client left the server" << std::endl;
			break; // to delete
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
	// check le premier mot pour connaitre la commande
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
	static int suffix = 0;
	// si getClients size == 0 -> suffix = 0
	// (=> ca arrive quand /quit pour tous les clients mais pas quitter le server)
	ClientIterator it = theServer.getClients().begin();
	++it;
	ClientIterator ite = theServer.getClients().end();
	for (; it != ite; ++it)
	{
		if (this->getNickname() == (*it)->getNickname())
		{
			this->setNickname(this->getNickname() + intToString(suffix));
			suffix++;
			return;
		}
	}
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
		// check nickname:
		// - sameNickname
		// - length of 9 max
		// - bloquer nombre de clients avant atoi max
		sameNickname(theServer);
		// check password:
		if (badPassword(theServer))
			return;
		std::string welcome_msg = welcome_client(this->getNickname(), this->getUsername());
		send(this->getSocket(), welcome_msg.c_str(), welcome_msg.length(), 0);
		this->isWelcome = false;
	}
}
