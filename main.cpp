/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 12:18:23 by araiteb           #+#    #+#             */
/*   Updated: 2024/04/15 12:09:48 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server/Server.hpp"

int 	checkpass(std::string str){
	if (str.empty()){
		std::cerr << "pass empty." << std::endl;
		return 0;
	}
	size_t i = 0;
	while(str[i]){
		if (str[i] == ' '){
			std::cerr << "pass invalide." << std::endl;
			return 0;
		}
		i++;
	}
	return 1;
}
int		checkport(std::string str){
	size_t i = 0;
	while (str[i])
	{
		if (!isdigit(str[i])){
			std::cerr << "port invalide." << std::endl;
			return (0);
		}
		i++;
	}
	if(std::atoi(str.c_str()) > 65535 || std::atoi(str.c_str()) < 1024){
		std::cerr << "port invalide." << std::endl;
		return (0);
	}
	return (1);
}
int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "prob in arguments" << std::endl;
		return 0;
	}
	if (!checkpass(av[2]) || !checkport(av[1]))
		return 0;
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