/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewu <jewu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:02:04 by jewu              #+#    #+#             */
/*   Updated: 2025/04/22 15:01:31 by jewu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "parsing.hpp"

//####
//#Constructor & Destructor
//##################

Server::Server(int port, std::string password)
{
	this->_port = port;
	this->_password = password;
}

Server::~Server(){
	std::vector<clientPair>::iterator it = this->_clients.begin();
	std::vector<clientPair>::iterator ite = this->_clients.end();
	for (; it != ite; ++it)
	{
		delete it->second;
		this->_clients.erase(it);
	}
	this->_clients.clear();
}

//####
//#Getters & Setters
//##################

std::string Server::getPassword(void)
{
	return this->_password;
}

int Server::getPort(void)
{
	return this->_port;
}

int Server::getSocket(void)
{
	return this->_socketfd;
}

std::vector<clientPair>& Server::getClients(void)
{
	return this->_clients;
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

	set_socket_non_blocking(this->_socketfd);

	struct sockaddr_in address;
	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(_port);
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
}

void Server::addNewClient(int epoll_fd, struct epoll_event& ev)
{
	int clientfd = accept(this->getSocket(), NULL, NULL);
	if (clientfd == -1)
	{
		throw std::runtime_error("Error: client connexion failure");
	}
	set_socket_non_blocking(clientfd);
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = clientfd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientfd, &ev) == -1)
	{
		throw std::runtime_error("Error: epoll_ctl failure");
	}
	this->getClients().push_back(clientPair(clientfd, new Client(clientfd)));
	std::cout << "New client connected: " << clientfd << std::endl;
}