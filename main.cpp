/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:25:58 by jewu              #+#    #+#             */
/*   Updated: 2025/05/19 13:44:59 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "parsing.hpp"

static void epoll_loop(Server& theServer, struct epoll_event& ev, struct epoll_event* events, int epoll_fd)
{
	int waitingfd;
	while (g_signal == 0)
	{
		waitingfd = epoll_wait(epoll_fd, events, MAX_CLIENTS, -1);
		if (waitingfd == -1)
			throw std::runtime_error("Error: epoll_wait failure");
		for (int i = 0; i < waitingfd; ++i)
		{
			int currentClientfd = events[i].data.fd;
			if (currentClientfd == theServer.getSocket())
			{
				theServer.addNewClient(epoll_fd, ev);
				continue;
			}
			ClientIterator client = theServer.findClient(currentClientfd);
			if (client == theServer.getClients().end())
				continue;
			if (events[i].events & (EPOLLERR | EPOLLHUP))
			{
				(*client)->deleteMessages();
				quit(*client, theServer);
				sendServerReply(*(*client));
				continue;
			}
			if (events[i].events & EPOLLOUT)
				sendServerReply(*(*client));
			if (events[i].events & EPOLLIN)
				(*client)->readClientMessage(theServer);
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
	std::cout << BOLD YELLOW "Server listening on port: " RESET << theServer.getPort() << std::endl;
	return epoll_fd;
}

int main(int argc, char **argv)
{
	if (check_arguments(argc, argv) == FALSE)
		return EXIT_FAILURE;
	try
	{
		handle_signals();
		int port = std::atoi(argv[1]);
		Server theServer(port, argv[2]);
		theServer.launch_angrybots_server();
		struct epoll_event ev, events[MAX_CLIENTS];
		std::memset(&ev, 0, sizeof(ev));
		theServer.setEpollfd(epoll_stuff(theServer, ev));
		epoll_loop(theServer, ev, events, theServer.getEpollfd());
	}catch(const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		if (g_signal != 0)
			return EXIT_SUCCESS;
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
