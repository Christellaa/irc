/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:02:04 by jewu              #+#    #+#             */
/*   Updated: 2025/04/28 12:58:34 by cde-sous         ###   ########.fr       */
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
	this->_epoll_fd = -1;
	this->_socketfd = -1;
}

Server::~Server(){
	{
		ClientIterator it = this->_clients.begin();
		ClientIterator ite = this->_clients.end();
		for (; it != ite; ++it)
		{
			close((*it)->getSocket());
			delete *it;
		}
		this->_clients.clear();
	}
	{
		ChannelIterator it = this->_channels.begin();
		ChannelIterator ite = this->_channels.end();
		for (; it != ite; ++it)
		{
			delete *it;
		}
		this->_channels.clear();
	}
	if (this->_epoll_fd != -1)
		close(this->_epoll_fd);
	if (this->_socketfd != -1)
		close(this->_socketfd);
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

ClientVec& Server::getClients(void)
{
	return this->_clients;
}

ChannelVec& Server::getChannels(void)
{
	return this->_channels;
}

int Server::getEpollfd(void)
{
	return this->_epoll_fd;
}

void Server::setEpollfd(int fd)
{
	this->_epoll_fd = fd;
}

//####
//#Exceptions
//##################

const char* Server::InvalidSocket::what() const throw(){
	return BOLD RED "Error: socket creation failure" RESET;
}

const char* Server::SetsockoptFailure::what() const throw(){
	return BOLD RED "Error: setsockopt failure" RESET;
}

const char* Server::BindingFailure::what() const throw(){
	return BOLD RED "Error: binding failure" RESET;
}

const char* Server::ListenFailure::what() const throw(){
	return BOLD RED "Error: listen failure" RESET;
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

	int reusePort = 1;
	if (setsockopt(this->_socketfd, SOL_SOCKET, SO_REUSEADDR, &reusePort, sizeof(reusePort)) == -1)
		throw Server::SetsockoptFailure();
	if (bind(_socketfd, (struct sockaddr*)&address, sizeof(address)) == INVALID_BIND)
		throw Server::BindingFailure();
	if (listen(_socketfd, 5) == INVALID_LISTEN)
		throw Server::ListenFailure();
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
	if (this->getClients().size() >= MAX_CLIENTS)
	{
		const char* msg = "Server full. Try again later.\n";
		send(clientfd, msg, strlen(msg), 0);
		close(clientfd);
		return;
	}
	set_socket_non_blocking(clientfd);
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = clientfd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientfd, &ev) == -1)
	{
		throw std::runtime_error("Error: epoll_ctl failure");
	}
	this->getClients().push_back(new Client(clientfd));
	std::cout << "New client connected on fd " << clientfd << std::endl;
}

