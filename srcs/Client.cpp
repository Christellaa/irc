/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:02:32 by jewu              #+#    #+#             */
/*   Updated: 2025/04/23 14:44:02 by codespace        ###   ########.fr       */
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


void 		Client::setPassword(std::string& password) { this->_password = password;}
void 		Client::setNickname(std::string& nickname) { this->_nickname = nickname;}
void 		Client::setUsername(std::string& username) { this->_username = username;}

//####
//#Exceptions
//##################

//####
//#Functions
//##################

Client* Client::findClient(std::vector<Client*>& clients, int clientfd)
{
	std::vector<Client*>::iterator it = clients.begin();
	std::vector<Client*>::iterator ite = clients.end();
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
	if (this->isWelcome){
		parseWelcomeMessage();
	}
	// check le premier mot pour connaitre la commande
	std::istringstream iss(this->getMsg());
	std::string word;
	iss >> word;
	if (word == "JOIN")
		join(this, theServer, iss);
	this->getMsg().clear();
}

void Client::parseWelcomeMessage()
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
		std::string welcome_msg = welcome_client(this->getNickname(), this->getUsername());
		send(this->getSocket(), welcome_msg.c_str(), welcome_msg.length(), 0);
		this->isWelcome = false;
	}
}
