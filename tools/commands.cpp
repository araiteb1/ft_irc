/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 12:04:13 by araiteb           #+#    #+#             */
/*   Updated: 2024/04/15 12:13:58 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../channel/Channel.hpp"
#include <sstream>

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
			else if(!SplitedMsg[0].compare("JOIN"))
				cmdjoin(SplitedMsg, c);
			else if(!SplitedMsg[0].compare("KICK"))
				cmdkick(SplitedMsg, c);
			else if(!SplitedMsg[0].compare("PART"))
				cmdpart(SplitedMsg, c);
			else if(!SplitedMsg[0].compare("INVITE"))
				cmdinvite(SplitedMsg, c);
			else if(!SplitedMsg[0].compare("TOPIC"))
				cmdtopic(SplitedMsg, c);
			else if(!SplitedMsg[0].compare("MODE"))
				cmdmode(SplitedMsg, c);
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

	std::cout << "cmdknick" << std::endl;
	if (!c->getNick().empty())
		flag = 1;
	if (SplitedMsg.size() != 2 ||  SplitedMsg[1].empty())
		throw Myexception(ERR_NONICKNAMEGIVEN);
	std::size_t found = SplitedMsg[1].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]`|^{}");
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
	std::cout << "cmdpass" << std::endl;
	if (this->IsAuthorized(*c))
		throw Myexception(ERR_ALREADYREGISTRED);
	if (SplitedMsg.size() != 2)
		throw Myexception(ERR_NEEDMOREPARAMS);
	c->seTPass(SplitedMsg[1]);
}

void	Server::cmduser(Client *c, std::vector<std::string> &SplitedMsg)
{
	std::cout << "cmduser" << std::endl;
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

std::vector<std::string> split(std::string str, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delim))
        result.push_back(token);
    return result;
}

void	Server::cmdprivmsg(std::vector<std::string>& SplitedMsg, Client *c)
{
	if (SplitedMsg.size() < 3)
		throw Myexception(ERR_NEEDMOREPARAMS);
	std::vector<std::string> targets = split(SplitedMsg[1], ',');
    for (size_t i = 0; i < targets.size(); i++)
    {
        if (targets[i][0] == '#')
        {
            if (channels.find(targets[i]) == channels.end())
                throw Myexception(ERR_NOSUCHCHANNEL);
            Channel *ch = channels[targets[i]];
            if (!ch->isMember(c))
                throw Myexception(ERR_CANNOTSENDTOCHAN);
            std::string msg = ':' + c->getNick() + '!' + c->getusername() + '@' + c->gethostname() + " PRIVMSG " + targets[i] + " :" + SplitedMsg[2] + "\r\n";
            ch->broadcast(msg);
        }
        else
        {
            Client *target = getClientByNickname(targets[i]);
            if (!target)
                throw Myexception(ERR_NOSUCHNICK);
            std::string msg = ':' + c->getNick() + '!' + c->getusername() + '@' + c->gethostname() + " PRIVMSG " + target->getNick() + " :" + SplitedMsg[2] + "\r\n";
            target->sendMsg(msg);
        }
    }
}

void    Server::cmdnotice(std::vector<std::string>& SplitedMsg, Client *c)
{
    if (SplitedMsg.size() < 3)
        return ;
    std::vector<std::string> targets = split(SplitedMsg[1], ',');
    for (size_t i = 0; i < targets.size(); i++)
    {
        if (targets[i][0] == '#')
        {
            if (channels.find(targets[i]) == channels.end())
                return ;
            Channel *ch = channels[targets[i]];
            if (!ch->isMember(c))
                return ;
            std::string msg = ':' + c->getNick() + '!' + c->getusername() + '@' + c->gethostname() + " NOTICE " + targets[i] + " :" + SplitedMsg[2] + "\r\n";
            ch->broadcast(msg);
        }
        else
        {
            Client *target = getClientByNickname(targets[i]);
            if (!target)
                return ;
            std::string msg = ':' + c->getNick() + '!' + c->getusername() + '@' + c->gethostname() + " NOTICE " + target->getNick() + " :" + SplitedMsg[2] + "\r\n";
            target->sendMsg(msg);
        }
    }
}

void	Server::cmdjoin(std::vector<std::string>& SplitedMsg, Client *c)
{
	if (SplitedMsg.size() < 2)
		throw Myexception(ERR_NEEDMOREPARAMS);
	std::vector<std::string> names = split(SplitedMsg[1], ',');
	std::vector<std::string> keys = std::vector<std::string>(names.size(), "");
    if (SplitedMsg.size() > 2)
    {
        std::vector<std::string> tmp = split(SplitedMsg[2], ',');
        if (tmp.size() > keys.size())
            keys = tmp;
        else
            keys.insert(keys.begin(), tmp.begin(), tmp.end());
    }
    for (size_t i = 0; i < names.size(); i++)
    {
        Channel *ch;
        if (names[i][0] != '#')
            throw Myexception(ERR_NOSUCHCHANNEL);
        if (channels.find(names[i]) == channels.end())
        {
            if (names[i].find_first_of(",\a ") != std::string::npos)
                throw Myexception(ERR_BADCHANMASK);
            ch = new Channel(names[i], keys[i], c);
            if (!keys[i].empty())
                ch->setMode(MODE_CHANKEY);
            ch->setMode(MODE_TOPREST);
            channels[names[i]] = ch;
        }
        else
        {
            ch = channels[names[i]];
            if (ch->getMode() & MODE_CHANKEY && ch->getKey() != keys[i])
                throw Myexception(ERR_BADCHANNELKEY);
            if (ch->getMode() & MODE_USERLIM && ch->getLimit() <= ch->getMembers().size())
                throw Myexception(ERR_CHANNELISFULL);
            if (ch->getMode() & MODE_INVONLY && !ch->isInvited(c))
                throw Myexception(ERR_INVITEONLYCHAN);
            ch->addMember(c);
        }
        std::string msg = ':' + c->getNick() + '!' + c->getusername() + '@' + c->gethostname() + " JOIN " + names[i] + "\r\n";
        ch->broadcast(msg);
        if (!ch->getTopic().empty())
            c->sendMsg(name + " 332 " + c->getNick() + " " + names[i] + " :" + ch->getTopic() + "\r\n");
        c->sendMsg(name + " MODE " + names[i] + " +" + ch->getModeStr() + "\r\n");
        c->sendMsg(name + " 353 " + c->getNick() + " = " + names[i] + " :" + ch->getMemberList() + "\r\n");
        c->sendMsg(name + " 366 " + c->getNick() + " " + names[i] + " :End of /NAMES list\r\n");
    }
}

void    Server::cmdlist(std::vector<std::string>& SplitedMsg, Client *c)
{
    if (SplitedMsg.size() == 1)
    {
        for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
        {
            Channel *ch = it->second;
            c->sendMsg(name + " 322 " + c->getNick() + " " + ch->getName() + " " + int2string(ch->getMembers().size()) + " :" + ch->getTopic() + "\r\n");
        }
        c->sendMsg(name + " 323 " + c->getNick() + " :End of /LIST\r\n");
    }
    else
    {
        std::vector<std::string> names = split(SplitedMsg[1], ',');
        for (size_t i = 0; i < names.size(); i++)
        {
            if (channels.find(names[i]) == channels.end())
                throw Myexception(ERR_NOSUCHCHANNEL);
            Channel *ch = channels[names[i]];
            c->sendMsg(name + " 322 " + c->getNick() + " " + ch->getName() + " " + int2string(ch->getMembers().size()) + " :" + ch->getTopic() + "\r\n");
        }
        c->sendMsg(name + " 323 " + c->getNick() + " :End of /LIST\r\n");
    }
}

void	Server::cmdkick(std::vector<std::string>& SplitedMsg, Client *c)
{
    if (SplitedMsg.size() < 3)
        throw Myexception(ERR_NEEDMOREPARAMS);
    std::vector<std::string> names = split(SplitedMsg[1], ',');
    std::vector<std::string> targets = split(SplitedMsg[2], ',');
    if (names.size() == 1 && targets.size() > 1)
        names = std::vector<std::string>(targets.size(), names[0]);
    else if (names.size() > 1 && targets.size() == 1)
        targets = std::vector<std::string>(names.size(), targets[0]);
    else if (names.size() != targets.size())
        throw Myexception(ERR_NEEDMOREPARAMS);
    for (size_t i = 0; i < names.size(); i++)
    {
        if (names[i][0] != '#')
            throw Myexception(ERR_NOSUCHCHANNEL);
        if (channels.find(names[i]) == channels.end())
            throw Myexception(ERR_NOSUCHCHANNEL);
        Channel *ch = channels[names[i]];
        if (!ch->isMember(c))
            throw Myexception(ERR_NOTONCHANNEL);
        if (!ch->isOperator(c))
            throw Myexception(ERR_CHANOPRIVSNEEDED);
        Client *target = getClientByNickname(targets[i]);
        if (!target)
            throw Myexception(ERR_NOSUCHNICK);
        if (!ch->isMember(target))
            throw Myexception(ERR_USERNOTINCHANNEL);
        std::string msg = ':' + c->getNick() + '!' + c->getusername() + '@' + c->gethostname() + " KICK " + names[i] + ' ' + target->getNick() + ':';
        msg += SplitedMsg.size() > 3 ? SplitedMsg[3] : "for some reason";
        msg += "\r\n";
        ch->broadcast(msg);
        ch->removeMember(target);
    }
}

void    Server::cmdpart(std::vector<std::string>& SplitedMsg, Client *c)
{
    if (SplitedMsg.size() < 2)
        throw Myexception(ERR_NEEDMOREPARAMS);
    std::vector<std::string> names = split(SplitedMsg[1], ',');
    for (size_t i = 0; i < names.size(); i++)
    {
        if (channels.find(names[i]) == channels.end())
            throw Myexception(ERR_NOSUCHCHANNEL);
        Channel *ch = channels[names[i]];
        if (!ch->isMember(c))
            throw Myexception(ERR_NOTONCHANNEL);
        std::string msg = ':' + c->getNick() + '!' + c->getusername() + '@' + c->gethostname() + " PART " + names[i];
        msg += SplitedMsg.size() > 2 ? " :" + SplitedMsg[2] : "";
        msg += "\r\n";
        ch->broadcast(msg);
        ch->removeMember(c);
    }
}

void	Server::cmdinvite(std::vector<std::string>& SplitedMsg, Client *c)
{
    if (SplitedMsg.size() < 3)
        throw Myexception(ERR_NEEDMOREPARAMS);
    Client *target = getClientByNickname(SplitedMsg[1]);
    if (target == NULL)
        throw Myexception(ERR_NOSUCHNICK);
    if (channels.find(SplitedMsg[2]) == channels.end())
        throw Myexception(ERR_NOSUCHCHANNEL);
    Channel *ch = channels[SplitedMsg[2]];
    if (!ch->isMember(c))
        throw Myexception(ERR_NOTONCHANNEL);
    if (ch->getMode() & MODE_INVONLY && !ch->isOperator(c))
        throw Myexception(ERR_CHANOPRIVSNEEDED);
    if (ch->isMember(target))
        throw Myexception(ERR_USERONCHANNEL);
    ch->addInvited(target);
    std::string msg = ':' + c->getNick() + '!' + c->getusername() + '@' + c->gethostname() + " INVITE " + target->getNick() + ' ' + SplitedMsg[2] + "\r\n";
    target->sendMsg(msg);
}

void    Server::cmdtopic(std::vector<std::string>& SplitedMsg, Client *c)
{
    if (SplitedMsg.size() < 2)
        throw Myexception(ERR_NEEDMOREPARAMS);
    if (channels.find(SplitedMsg[1]) == channels.end())
        throw Myexception(ERR_NOSUCHCHANNEL);
    Channel *ch = channels[SplitedMsg[1]];
    if (!ch->isMember(c))
        throw Myexception(ERR_NOTONCHANNEL);
    if (SplitedMsg.size() == 2)
    {
        std::string msg = name + " 331 " + c->getNick() + " " + SplitedMsg[1] + " :";
        msg += ch->getTopic().empty() ? "No topic is set" : ch->getTopic();
        msg += "\r\n";
        c->sendMsg(msg);
    }
    else
    {
        if (ch->getMode() & MODE_TOPREST && !ch->isOperator(c))
            throw Myexception(ERR_CHANOPRIVSNEEDED);
        ch->setTopic(SplitedMsg[2]);
        std::string msg = ':' + c->getNick() + '!' + c->getusername() + '@' + c->gethostname() + " TOPIC " + SplitedMsg[1] + " :" + SplitedMsg[2] + "\r\n";
        ch->broadcast(msg);
    }
}

void    Server::cmdmode(std::vector<std::string>& SplitedMsg, Client *c)
{
    if (SplitedMsg.size() < 2)
        throw Myexception(ERR_NEEDMOREPARAMS);
    if (channels.find(SplitedMsg[1]) == channels.end())
        throw Myexception(ERR_NOSUCHCHANNEL);
    Channel *ch = channels[SplitedMsg[1]];
    if (!ch->isMember(c))
        throw Myexception(ERR_NOTONCHANNEL);
    if (SplitedMsg.size() == 2)
        c->sendMsg(name + " 324 " + c->getNick() + " " + SplitedMsg[1] + " +" + ch->getModeStr() + "\r\n");
    else
    {
        if (!ch->isOperator(c))
            throw Myexception(ERR_CHANOPRIVSNEEDED);
        if (SplitedMsg[2] == "+sn")
            return ;
        size_t  arg = 3;
        char    operation = 0;
        for (size_t i = 0; i < SplitedMsg[2].size(); i++)
        {
            if (SplitedMsg[2][i] == '-' || SplitedMsg[2][i] == '+')
                operation = SplitedMsg[2][i++];
            if (operation == '+')
            {
                if (SplitedMsg[2][i] == 'i')
                    ch->setMode(MODE_INVONLY);
                else if (SplitedMsg[2][i] == 't')
                    ch->setMode(MODE_TOPREST);
                else
                {
                    if (SplitedMsg.size() < arg + 1)
                        throw Myexception(ERR_NEEDMOREPARAMS);
                    if (SplitedMsg[2][i] == 'k')
                    {
                        ch->setKey(SplitedMsg[arg++]);
                        ch->setMode(MODE_CHANKEY);
                    }
                    else if (SplitedMsg[2][i] == 'l')
                    {
                        ch->setLimit(std::atoi(SplitedMsg[arg++].c_str()));
                        ch->setMode(MODE_USERLIM);
                    }
                    else if (SplitedMsg[2][i] == 'o')
                    {
                        Client *target = getClientByNickname(SplitedMsg[arg++]);
                        if (!target)
                            throw Myexception(ERR_NOSUCHNICK);
                        if (!ch->isMember(target))
                            throw Myexception(ERR_USERNOTINCHANNEL);
                        ch->setOperator(target);
                    }
                    else
                        throw Myexception(ERR_UNKNOWNCOMMAND);
                }
            }
            else if (operation == '-')
            {
                if (SplitedMsg[2][i] == 'i')
                    ch->unsetMode(MODE_INVONLY);
                else if (SplitedMsg[2][i] == 't')
                    ch->unsetMode(MODE_TOPREST);
                else if (SplitedMsg[2][i] == 'k')
                {
                    ch->setKey("");
                    ch->unsetMode(MODE_CHANKEY);
                }
                else if (SplitedMsg[2][i] == 'l')
                {
                    ch->setLimit(0);
                    ch->unsetMode(MODE_USERLIM);
                }
                else if (SplitedMsg[2][i] == 'o')
                {
                    if (SplitedMsg.size() < arg + 1)
                        throw Myexception(ERR_NEEDMOREPARAMS);
                    Client *target = getClientByNickname(SplitedMsg[arg++]);
                    if (!target)
                        throw Myexception(ERR_NOSUCHNICK);
                    if (!ch->isMember(target))
                        throw Myexception(ERR_USERNOTINCHANNEL);
                    ch->unsetOperator(target);
                }
                else
                    throw Myexception(ERR_UNKNOWNCOMMAND);
            }
            else
                throw Myexception(ERR_UNKNOWNCOMMAND);
        }
        std::string msg = ':' + c->getNick() + '!' + c->getusername() + '@' + c->gethostname() + " MODE +" + ch->getModeStr() + "\r\n";
        ch->broadcast(msg);
    }
}