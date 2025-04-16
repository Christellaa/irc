/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewu <jewu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:02:32 by jewu              #+#    #+#             */
/*   Updated: 2025/04/16 15:24:13 by jewu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

//####
//#Constructord & Destructors
//##################

Client::Client(): _socketfd(), _nickname(), _username(), _password(){}

Client::Client(int socketfd): _socketfd(socketfd) {}

Client::Client(int socketfd, std::string nickname, std::string username, std::string password)
{
	this->_socketfd = socketfd;
	this->_nickname = nickname;
	this->_username = username;
	this->_password = password;
}

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


//####
//#Exceptions
//##################

//####
//#Functions
//##################