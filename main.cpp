/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewu <jewu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:25:58 by jewu              #+#    #+#             */
/*   Updated: 2025/04/16 15:20:50 by jewu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "parsing.hpp"

static void epoll_loop(Server& theServer, struct epoll_event& ev, struct epoll_event* events, int epoll_fd)
{
	int waitingfd;
	while (1)
	{
		waitingfd = epoll_wait(epoll_fd, events, MAX_CLIENTS, -1);
		if (waitingfd == -1)
		{
			throw std::runtime_error("Error: epoll_wait failure");
		}
		for (int i = 0; i < waitingfd; ++i)
		{
			int currentClientfd = events[i].data.fd;
			if (currentClientfd == theServer.getSocket())
			{
				int clientfd = accept(theServer.getSocket(), NULL, NULL);
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
				// ajouter dans classe client
				theServer.getClients().insert(std::pair<int, Client*>(clientfd, new Client(clientfd)));
				std::cout << "New client connected: " << clientfd << std::endl;
			}
			else
			{
				std::map<int, Client*>::iterator it = theServer.getClients().find(currentClientfd);
				if (it == theServer.getClients().end())
					break; // couldn't find client in map => prob an error
				Client& client = *it->second;
				char buffer[MAX_CHAR_MSG];
				while (1)
				{
					ssize_t bytes = recv(currentClientfd, buffer, sizeof(buffer), 0);
					if (bytes == -1)
					{
						if (errno == EAGAIN || errno == EWOULDBLOCK)
							break;
						else
							throw std::runtime_error("recv failed");
					}
					else if (bytes == 0)
					{
						std::cout << "parsing: " << client.getMsg() << std::endl;
						break;
					}
					else
					{
						buffer[bytes] = '\0';
						client.getMsg().append(buffer, bytes);
						std::cout << "Message: " << client.getMsg() << std::endl;
					}
 				}
			}
		}
	}
}

static int epoll_stuff(Server& theServer, struct epoll_event& ev)
{
	int epoll_fd  = epoll_create1(0);
	if (epoll_fd == -1)
	{
		throw std::runtime_error("Error: epoll failure");
	}
	ev.events = EPOLLIN;
	ev.data.fd = theServer.getSocket();
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, theServer.getSocket(), &ev) == -1)
	{
		throw std::runtime_error("Error: epoll_ctl failure");
	}
	std::cout << "Server listening on port: " << theServer.getPort() << std::endl;
	return epoll_fd;
}

int main(int argc, char **argv)
{
	if (check_arguments(argc, argv) == FALSE)
		return EXIT_FAILURE;
	try
	{
		int port = std::atoi(argv[1]);
		Server theServer(port, argv[2]);
		theServer.launch_angrybots_server();
		struct epoll_event ev, events[MAX_CLIENTS];
		int epoll_fd = epoll_stuff(theServer, ev);
		epoll_loop(theServer, ev, events, epoll_fd);
		// boucle d'ecoute du server
	}catch(const std::exception& e)
	{
		std::cout << "Exception caught: " << e.what() << std::endl;
		//clean()
		exit(EXIT_FAILURE);
	}
	
	return EXIT_SUCCESS;
}
