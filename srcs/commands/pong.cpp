/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:28:05 by jewu              #+#    #+#             */
/*   Updated: 2025/05/07 14:17:51 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void pong(Client& client, std::istringstream& iss)
{
	std::string word;
	iss >> word;
	std::string answer = PONG(client.getNickname(), word);
	// std::string answer = ":" + client.getNickname() + " PONG :" + word + "\r\n";
	send(client.getSocket(), answer.c_str(), answer.length(), 0);
	std::cout << answer << std::endl;
}
