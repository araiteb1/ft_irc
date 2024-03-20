/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 12:04:13 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/20 13:07:07 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"

void	Server::commands(Message &msg, std::vector <std::string> &SplitedMsg)
{
    Client *c ;

    c = getClientByFd(msg.getSenderFd());
    if (!c)
        return ;
    for (int i = 0 ; SplitedMsg[0][i] ; i++)
		SplitedMsg[0][i] = toupper(SplitedMsg[0][i]);
    try
    {
        if (!SplitedMsg[0].compare("PASS"))
            cmdpass(SplitedMsg, c);
        else if (!SplitedMsg[0].compare("NICK"))
            cmdknick(SplitedMsg, c);
        else if (!SplitedMsg[0].compare("USER"))
            cmduser(c, SplitedMsg);
        else if (this->IsAuthorized(*c))
        {
            if(!SplitedMsg[0].compare("PRIVMSG"))
                cmdprivmsg(SplitedMsg, c);
        }
        else
            throw Myexception(ERR_ALREADYREGISTRED);
    }
        catch(Myexception & e) {
        sendResponce(c->getFd(), this->name
            + int2string(e.getERROR_NO()) + " "
            + c->getNick() + " "
            + SplitedMsg[0] + " "
            + e.what() + "\n");
         if (this->IsAuthorized(*c) == 2)
            this->clientLeft(c->getFd());
    }	
}

void	Server::cmdknick(std::vector<std::string> &SplitedMsg, Client *c)
{
   Client *tmpClient;
    int flag = 0;

    if (!c->getNick().empty())
        flag = 1;
    if (SplitedMsg.size() != 2 ||  SplitedMsg[1].empty())
        throw Myexception(ERR_NONICKNAMEGIVEN);
    std::size_t found = SplitedMsg[1].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]`|/^{}");
    if (found != std::string::npos)
        throw Myexception(ERR_ERRONEUSNICKNAME);
    if (!SplitedMsg[1].empty() && SplitedMsg[2].empty())
    {
        tmpClient = this->getClientByNickname(SplitedMsg[1]);
        if (tmpClient && (tmpClient->getFd() != c->getFd() || !SplitedMsg[1].compare("Bot")))
            throw Myexception(ERR_NICKNAMEINUSE);
        c->seTNick(SplitedMsg[1]);
        if (this->IsAuthorized(*c) == 2)
            throw  Myexception(ERR_PASSWDMISMATCH);
        if (this->IsAuthorized(*c) == 1 && !flag)
        {
            sendResponce(c->getFd(), this->name + "001 "
                + c->getNick() + " :Welcome to the Internet Relay Network "
                + c->getIdent() + "\n");
            sendResponce(c->getFd(), this->name + "002 "
                + c->getNick() + " :Your host is "
                + this->name.substr(1) + "running on version 0.1 \n");
            sendResponce(c->getFd(), this->name + "003 "
                + c->getNick() + " :This server was created "
                + this->birthday);
        }
    }
}

void	Server::cmdpass(std::vector<std::string>& SplitedMsg, Client *c)
{
    if (this->IsAuthorized(*c))
        throw Myexception(ERR_ALREADYREGISTRED);
	if (SplitedMsg.size() != 2)
		throw Myexception(ERR_NEEDMOREPARAMS);
	c->seTPass(SplitedMsg[1]);
}


void	Server::cmdprivmsg(std::vector<std::string>& SplitedMsg, Client *c)
{
	Client *newClient;
    
	newClient = getClientByNickname(SplitedMsg[1]);
	if (!newClient)
		throw Myexception(ERR_NOSUCHNICK);
    else if (SplitedMsg.size() < 3)
        throw Myexception(ERR_NEEDMOREPARAMS);
    else if (SplitedMsg.size() == 1)
        throw Myexception(ERR_NORECIPIENT);
	else if (!SplitedMsg[2].empty()) {
        if (!c->getNick().compare("Bot")){
            if (!SplitedMsg[3].empty() && !SplitedMsg[4].empty()){
                std::string msg = ":" + c->getNick()+ " " + SplitedMsg[0] + " " + SplitedMsg[1] + " :" + SplitedMsg[2] +":" +SplitedMsg[3] + ":" + SplitedMsg[4];
                sendResponce(newClient->getFd(), msg);
            }
            else{
                std::string msg = ":" + c->getNick()+ " " + SplitedMsg[0] + " " + SplitedMsg[1] + " :" + SplitedMsg[2];
		        sendResponce(newClient->getFd(), msg);
            }
        }
        else{
		    std::string msg = ":" + c->getNick()+ " " + SplitedMsg[0] + " " + SplitedMsg[1] + " :" + SplitedMsg[2];
		    sendResponce(newClient->getFd(), msg);
            
        }
        sendResponce(newClient->getFd(), "\n");
	}
}

void	Server::cmduser(Client *c, std::vector<std::string> &SplitedMsg)
{
      if (this->IsAuthorized(*c) || !c->getusername().empty())
        throw Myexception(ERR_ALREADYREGISTRED);
	if (SplitedMsg.size() != 5)
		throw Myexception(ERR_NEEDMOREPARAMS);
    std::size_t found = SplitedMsg[1].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]`|^{}");
    if (found != std::string::npos)
        throw Myexception(ERR_ERRONEUSNICKNAME);
	c->setusename(SplitedMsg[1]);
    c->sethostname(SplitedMsg[2]);
    c->setservername(SplitedMsg[3]);
    c->setrealname(SplitedMsg[4]);
    if (this->IsAuthorized(*c) == 2)
        throw  Myexception(ERR_PASSWDMISMATCH);
	if (this->IsAuthorized(*c) == 1) 
    {
        sendResponce(c->getFd(), this->name + "001 "
            + c->getNick() + " :Welcome to the Internet Relay Network "
            + c->getIdent() + "\n");
        sendResponce(c->getFd(), this->name + "002 "
            + c->getNick() + " :Your host is "
            + this->name.substr(1) + "running on version 0.1 \n");
        sendResponce(c->getFd(), this->name + "003 "
            + c->getNick() + " :This server was created "
            + this->birthday);
    }
}