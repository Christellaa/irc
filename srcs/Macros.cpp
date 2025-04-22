/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Macros.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewu <jewu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:38:01 by jewu              #+#    #+#             */
/*   Updated: 2025/04/22 13:38:23 by jewu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Macros.hpp"

std::string welcome_client(std::string nickname, std::string username)
{
	return ":ircserv 001 " + nickname + " :Welcome, " + username + "\n";
}