/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anammal <anammal@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 10:23:41 by araiteb           #+#    #+#             */
/*   Updated: 2024/04/18 21:23:36 by anammal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#define MAX 1024
#define LIMITCNX 500

#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <cstring>
#include <fcntl.h>
#include <cstddef>
#include <csignal>
#include <sstream>
#include <algorithm>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <arpa/inet.h>
#include "../clients/Client.hpp"
#include "../message/Message.hpp"
#include "../exception/Exception.hpp"
#include "../channel/Channel.hpp"

class Client;
class Message;
class Channel;

typedef std::map<std::string, Channel *> channelMap;

class Server {
	
	private:
		char *						birthday;
		std::string					name;
		std::string					m_pass;
		char						buffer[MAX];
		int							m_port;
		int							m_socket;
		int							server_fd;
		int							on;
		int							user_num;
		int							num;
		int							end_con;
		sockaddr_in					address;
		struct pollfd				users[LIMITCNX];
		int							len;
		long int					timeout;
		int							end_ser;
		int							compress_array;
		std::string					msgsendResponce;
		std::string					checkPiv;
		std::map <int, Client *>	clients;
		channelMap					channels;

    public:
		static Server	*serverPtr;
	
        Server(const std::string ipAdd, int port);
		Server(Server &sr);
		Server& operator=(const Server &sr);
		~Server();

        void			seTpass(const std::string);
		void 			seTport(int p);
		void 			seTmsocket(int sock);
		void 			seTServerFd(int server);
		void 			seTmapClient(std::map<int, Client *>);

        std::string				getPass();
		int 					getport();
		int 					getsocket();
		int 					getSeverSocket();
		std::map<int, Client *> getClient();
		std::string 			getNick();

        int				CreateSocket();
		int				OptionSocket();
		int				NnBlockFd();
		int				BindSocket();
		int				listenSocket();
		int 			acceptingData();
		int 			checkmsg(int i);
		void			PollingFd();
        
        void			cmduser(Client *c, std::vector<std::string> &SplitedMsg);
		void			cmdpass(std::vector<std::string>& SplitedMsg, Client *c);
		void			cmdknick(std::vector<std::string> &SplitedMsg, Client *c);
		void			cmdprivmsg(std::vector<std::string>& SplitedMsg, Client *c);
		void			cmdnotice(std::vector<std::string>& SplitedMsg, Client *c);
		void			cmdjoin(std::vector<std::string>& SplitedMsg, Client *c);
		void			cmdlist(std::vector<std::string>& SplitedMsg, Client *c);
		void			cmdmode(std::vector<std::string>& SplitedMsg, Client *c);
		void			cmdkick(std::vector<std::string>& SplitedMsg, Client *c);
		void			cmdpart(std::vector<std::string>& SplitedMsg, Client *c);
		void			cmdinvite(std::vector<std::string>& SplitedMsg, Client *c);
		void			cmdtopic(std::vector<std::string>& SplitedMsg, Client *c);
		void			cmdping(std::vector<std::string>& SplitedMsg, Client *c);
		void			cmdpong(std::vector<std::string>& SplitedMsg, Client *c);
		void			cmdwho(std::vector<std::string>& SplitedMsg, Client *c);

		void			cmdquit(std::vector<std::string>& SplitedMsg, Client *c);
		void			cmdquit(Client *c, std::string reason);

        Client*			getClientByFd(int fdUser);
		Client*			getClientByNickname(std::string nickname);
		int				IsAuthorized(Client &);
		void			TraiteMessage(Message &);

		void 			clientLeft(int fd);
		void 			quitServer();

		void			commands(Message &msg, std::vector <std::string> &SplitedMsg);
		std::string 	to_string(int number);

		static void		signal_handler(int signal);

};

int	split(std::string msg, std::vector<std::string> &SplitedMsg);
void    splitCommand(std::string str, char oper, std::vector<std::string> &SplitedMsg);
void	initTab(std::string strs[MAX]);
void 	sendResponce(int fd, const std::string &responce);
std::string const   int2string (int n);
#endif
