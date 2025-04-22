/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewu <jewu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:02:32 by jewu              #+#    #+#             */
/*   Updated: 2025/04/22 13:46:41 by jewu             ###   ########.fr       */
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


void 		Client::setPassword(std::string& password) { this->_password = password;}
void 		Client::setNickname(std::string& nickname) { this->_nickname = nickname;}
void 		Client::setUsername(std::string& username) { this->_username = username;}

//####
//#Exceptions
//##################

//####
//#Functions
//##################

Client* Client::findClient(std::vector<clientPair>& clients, int clientfd)
{
	std::vector<clientPair>::iterator it = clients.begin();
	std::vector<clientPair>::iterator ite = clients.end();
	for (; it != ite; ++it)
	{
		if (clientfd == it->first)
			return it->second;
	}
	return NULL;
}

void Client::parseClientMessage(int currentClientfd)
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
		send(currentClientfd, welcome_msg.c_str(), welcome_msg.length(), 0);
		this->isWelcome = false;
	}
	this->getMsg().clear();
}

void Client::readClientMessage(int currentClientfd)
{
	char buffer[MAX_CHAR_MSG];
	while (1)
	{
		if (this->getMsg().find("\n") != std::string::npos)
			break;
		ssize_t bytes = recv(currentClientfd, buffer, sizeof(buffer), MSG_DONTWAIT);
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
	parseClientMessage(currentClientfd);
}