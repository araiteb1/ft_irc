/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 10:28:26 by araiteb           #+#    #+#             */
/*   Updated: 2024/04/15 15:02:10 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
	this->nickname = "";
	this->pass = "";
	this->username = "";
	this->hostname = "";
	this->servername = "";
	this->realname = "";
}
Client::Client(int fd)
{
	std::cout << "New User Added !" << std::endl;
	this->fd = fd;
	this->nickname = "";
	this->pass = "";
	this->username = "";
	this->hostname = "";
	this->servername = "";
	this->realname = "";
}
Client::Client(Client &cl)
{
	*this = cl;
}
Client& Client::operator=(const Client &cl)
{
	nickname = cl.nickname;
	pass = cl.pass;
	this->username = cl.username;
	this->hostname = cl.hostname;
	this->realname = cl.realname;
	this->servername = cl.servername;
	return (*this);
}
Client::~Client()
{
	
}
void    Client::seTNick(std::string nick)
{
	nickname = nick;
}

void    Client::seTPass(std::string pwd)
{
	pass = pwd;
}
void    Client::setFdUser(int FdUser)
{
	fd = FdUser;
}

std::string &     Client::getNick() { return (nickname); }

std::string &     Client::geTPass(){ return (pass); }

void	Client::setusename(std::string val) { this->username = val; }
void	Client::sethostname(std::string val) { this->hostname = val; }
void	Client::setservername(std::string val) { this->servername = val; }
void	Client::setrealname(std::string val) { this->realname = val; }

std::string &		Client::getusername(){
	return this->username;
}
std::string &		Client::gethostname(){
	return this->hostname;
}
std::string &		Client::getservername(){
	return this->servername;
}
std::string &		Client::getrealname(){
	return this->realname;
}

int             Client::getFd() {
	return this->fd;
}

std::string		Client::getIdent(){
	return (':' + getNick() + "!" + getusername()+ "@" + gethostname());
}

void			Client::sendMsg(std::string msg)
{
	send(fd, msg.c_str(), msg.length(), 0);
}
