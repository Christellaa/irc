/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:00:31 by jewu              #+#    #+#             */
/*   Updated: 2025/05/19 12:18:26 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Macros.hpp"

volatile int g_signal = 0;

void	sigint(int code)
{
	g_signal = 128 + code;
	throw SignalQuit();
}

void	sigquit(int code)
{
	g_signal = 128 + code;
	throw SignalQuit();
}

void	sigtstp(int code)
{
	g_signal = 128 + code;
	throw SignalQuit();
}

void	handle_signals(void)
{
	signal(SIGINT, sigint);
	signal(SIGQUIT, sigquit);
	signal(SIGTSTP, sigtstp);
}
