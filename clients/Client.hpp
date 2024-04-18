/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 10:28:14 by araiteb           #+#    #+#             */
/*   Updated: 2024/04/18 23:03:05 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include<iostream>
#include<string>
#include <vector>
#include "../server/Server.hpp"

class Client
{
	private:
		std::string		nickname;
		std::string		pass;
		std::string		username;
		std::string		hostname;
		std::string		servername;
		std::string		realname;
		std::string		remain;
		int fd;
	public:
		Client();
		Client(int fd);
		Client(Client &cl);
		Client& operator =(const Client &cl);
		~Client();
	
		void			seTNick(std::string nick);
		void			seTPass(std::string pwd);
		void			setFdUser(int FdUser);
		void			setusename(std::string);
		void			sethostname(std::string);
		void			setservername(std::string);
		void			setrealname(std::string);
		void    		setremain(std::string rem);
		
		 
		int				getFd();
		std::string		&getNick();
		std::string		&geTPass();
		std::string		&getusername();
		std::string		&gethostname();
		std::string		&getservername();
		std::string		&getrealname();
		std::string 	&getremain();
		
		std::string 	getIdent();

		void			sendMsg(std::string msg);
};


#endif