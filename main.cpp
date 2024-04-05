/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 12:18:23 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/19 12:28:55 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server/Server.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "prob in arguments" << std::endl;
		return 0;
	}
	Server S(av[2], atoi(av[1]));
	if (!S.CreateSocket())
		exit (1);
	if (!S.OptionSocket())
		exit (1);
	if (!S.NnBlockFd())
		exit (1);
	if (!S.BindSocket())
		exit (1);
	if (!S.listenSocket())
		exit (1);
	S.PollingFd();
	return 0;
}