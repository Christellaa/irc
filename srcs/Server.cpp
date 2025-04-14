/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewu <jewu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:02:04 by jewu              #+#    #+#             */
/*   Updated: 2025/04/14 17:51:47 by jewu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//####
//#Constructors & Destructors
//##################

Server::Server() : _port(), _password(){}

Server::Server(std::string port, std::string password)
{
	this->_port = port;
	this->_password = password;
}

Server::~Server(){}

//####
//#Getters & Setters
//##################

std::string Server::getPassword(void)
{
	return this->_password;
}

std::string Server::getPort(void)
{
	return this->_port;
}

//####
//#Exceptions
//##################

const char* Server::InvalidSocket::what() const throw(){
	return BOLD RED "Socket creation failure" RESET;
}

const char* Server::BindingFailure::what() const throw(){
	return BOLD RED "Binding failure" RESET;
}

const char* Server::ListenFailure::what() const throw(){
	return BOLD RED "Listen failure" RESET;
}

//####
//#Functions
//##################

void Server::setting_server_socket(void)
{
	this->_socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socketfd < INVALID_SOCKET)
		throw Server::InvalidSocket();

	struct sockaddr_in address;
	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	long port_long = std::strtol(this->_port.c_str(), NULL, 10);
	uint16_t port_number = static_cast<uint16_t>(port_long);
	address.sin_port = htons(port_number);
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(_socketfd, (struct sockaddr*)&address, sizeof(address)) == INVALID_BIND)
	{
		close(_socketfd);
		throw Server::BindingFailure();
	}

	if (listen(_socketfd, 5) == INVALID_LISTEN)
	{
		close(_socketfd);
		throw Server::ListenFailure();
	}
}

void Server::launch_angrybots_server(void)
{
	std::cout << BOLD PURPLE "HELLO ANGRYBOTS" RESET << std::endl;
	std::cout << CYAN "SERVER PORT: " << RESET << getPort() << std::endl;
	std::cout << CYAN "SERVER PASSWORD: " << RESET << getPassword() << std::endl;

	setting_server_socket();
	//bind
	//listen
}