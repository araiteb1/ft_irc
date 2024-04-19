/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 12:04:13 by araiteb           #+#    #+#             */
/*   Updated: 2024/04/19 18:24:54 by araiteb          ###   ########.fr       */
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
		else if (!SplitedMsg[0].compare("NICK") && !c->geTPass().empty())
			cmdknick(SplitedMsg, c);
		else if (!SplitedMsg[0].compare("USER") && !c->geTPass().empty())
			cmduser(c, SplitedMsg);
		else if (this->IsAuthorized(*c))
		{
			if(!SplitedMsg[0].compare("PRIVMSG"))
				cmdprivmsg(SplitedMsg, c);
			else if(!SplitedMsg[0].compare("NOTICE"))
				cmdnotice(SplitedMsg, c);
			else if(!SplitedMsg[0].compare("JOIN"))
				cmdjoin(SplitedMsg, c);
			else if(!SplitedMsg[0].compare("LIST"))
				cmdlist(SplitedMsg, c);
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
            else if(!SplitedMsg[0].compare("QUIT"))
                cmdquit(SplitedMsg, c);
            else if(!SplitedMsg[0].compare("PING"))
                cmdping(SplitedMsg, c);
            else if(!SplitedMsg[0].compare("PONG"))
                cmdpong(SplitedMsg, c);
            else if(!SplitedMsg[0].compare("WHO"))
                cmdwho(SplitedMsg, c);
            else
                throw Myexception(ERR_UNKNOWNCOMMAND, SplitedMsg);
            
		}
		else
			throw Myexception(ERR_ALREADYREGISTRED,SplitedMsg);
	}
		catch(Myexception & e) {
		sendResponce(c->getFd(), this->name
			+ int2string(e.getERROR_NO()) + " "
			+ c->getNick() + " "
			+ SplitedMsg[0] + " "
			+ e.what() + "\n");
        if (this->IsAuthorized(*c) == 2)
            cmdquit(c, "Connection closed");
	}	
}

void	Server::cmdknick(std::vector<std::string> &SplitedMsg, Client *c)
{
	Client *tmpClient;
	int flag = 0;

	if (!c->getNick().empty())
		flag = 1;
	if (SplitedMsg.size() > 0 && SplitedMsg.size() > 2)
		throw Myexception(ERR_NEEDMOREPARAMS, SplitedMsg);
	if (SplitedMsg.size() > 0 && SplitedMsg.size() == 1)
		throw Myexception(ERR_NONICKNAMEGIVEN, SplitedMsg);
	std::size_t found = SplitedMsg[1].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]`|^{}");
	if (found != std::string::npos)
		throw Myexception(ERR_ERRONEUSNICKNAME, SplitedMsg);
	if (!SplitedMsg[1].empty())
	{    
		tmpClient = this->getClientByNickname(SplitedMsg[1]);
		if (tmpClient && (tmpClient->getFd() != c->getFd() || !SplitedMsg[1].compare("Bot")))
			throw Myexception(ERR_NICKNAMEINUSE, SplitedMsg);
        std::string msg = c->getIdent() + " NICK " + SplitedMsg[1] + "\r\n";
		c->seTNick(SplitedMsg[1]);
		if (this->IsAuthorized(*c) == 2)
			throw  Myexception(ERR_PASSWDMISMATCH, SplitedMsg);
		if (this->IsAuthorized(*c) == 1)
		{
            if (flag)
            {
                for (channelMap::iterator it = channels.begin(); it != channels.end(); it++)
                {
                    Channel *ch = it->second;
                    if (ch->isMember(c))
                        ch->broadcast(msg);
                }
            }
            else
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
                sendResponce(c->getFd(), this->name + "004 "
                    + c->getNick() + " : "
                    + "0.1" + " iktlo klo\r\n");
                sendResponce(c->getFd(), this->name + "005 "
                    + "PREFIX=(o)@ CHANTYPES=# :are supported by this server\r\n");
                sendResponce(c->getFd(), this->name + "376 " + c->getNick() + " :End of /MOTD command.\n");
            }
		}
	}
}

void	Server::cmdpass(std::vector<std::string>& SplitedMsg, Client *c)
{
	if (this->IsAuthorized(*c))
		throw Myexception(ERR_ALREADYREGISTRED, SplitedMsg);
	if (SplitedMsg.size() != 2)
		throw Myexception(ERR_NEEDMOREPARAMS, SplitedMsg);
	c->seTPass(SplitedMsg[1]);
}

void	Server::cmduser(Client *c, std::vector<std::string> &SplitedMsg)
{
	if (this->IsAuthorized(*c) || !c->getusername().empty())
		throw Myexception(ERR_ALREADYREGISTRED, SplitedMsg);
	if (SplitedMsg.size() != 5)
		throw Myexception(ERR_NEEDMOREPARAMS, SplitedMsg);
	std::size_t found = SplitedMsg[1].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]`|^{}");
	if (found != std::string::npos)
		throw Myexception(ERR_ERRONEUSNICKNAME, SplitedMsg);
	c->setusename(SplitedMsg[1]);
	// c->sethostname(SplitedMsg[2]);
	c->setservername(SplitedMsg[3]);
	c->setrealname(SplitedMsg[4]);
	if (this->IsAuthorized(*c) == 2)
		throw  Myexception(ERR_PASSWDMISMATCH, SplitedMsg);
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
        sendResponce(c->getFd(), this->name + "004 "
            + c->getNick() + " : "
            + "0.1" + " iktlo klo\r\n");
        sendResponce(c->getFd(), this->name + "005 "
            + "PREFIX=(o)@ CHANTYPES=# :are supported by this server\r\n");
        sendResponce(c->getFd(), this->name + "376 " + c->getNick() + " :End of /MOTD command.\n");
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
		throw Myexception(ERR_NEEDMOREPARAMS, SplitedMsg);
	std::vector<std::string> targets = split(SplitedMsg[1], ',');
    for (size_t i = 0; i < targets.size(); i++)
    {
        if (targets[i][0] == '#')
        {
            if (channels.find(targets[i]) == channels.end())
                throw Myexception(ERR_NOSUCHCHANNEL, SplitedMsg);
            Channel *ch = channels[targets[i]];
            if (!ch->isMember(c))
                throw Myexception(ERR_CANNOTSENDTOCHAN, SplitedMsg);
            std::string msg = c->getIdent() + " PRIVMSG " + targets[i] + " :" + SplitedMsg[2] + "\r\n";
            ch->broadcast(msg, c);
        }
        else
        {
            Client *target = getClientByNickname(targets[i]);
            if (!target)
                throw Myexception(ERR_NOSUCHNICK, SplitedMsg);
            std::string msg = c->getIdent() + " PRIVMSG " + target->getNick() + " :" + SplitedMsg[2] + "\r\n";
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
            std::string msg = c->getIdent() + " NOTICE " + targets[i] + " :" + SplitedMsg[2] + "\r\n";
            ch->broadcast(msg);
        }
        else
        {
            Client *target = getClientByNickname(targets[i]);
            if (!target)
                return ;
            std::string msg = c->getIdent() + " NOTICE " + target->getNick() + " :" + SplitedMsg[2] + "\r\n";
            target->sendMsg(msg);
        }
    }
}

void	Server::cmdjoin(std::vector<std::string>& SplitedMsg, Client *c)
{
	if (SplitedMsg.size() < 2)
		throw Myexception(ERR_NEEDMOREPARAMS, SplitedMsg);
	std::vector<std::string> names = split(SplitedMsg[1], ',');
	std::vector<std::string> keys = std::vector<std::string>(names.size(), "");
    if (SplitedMsg.size() > 2)
    {
        std::vector<std::string> tmp = split(SplitedMsg[2], ',');
        std::copy(tmp.begin(), tmp.begin() + std::min(tmp.size(), keys.size()), keys.begin());
    }
    for (size_t i = 0; i < names.size(); i++)
    {
        try
        {
            Channel *ch;
            if (names[i][0] != '#')
                throw Myexception(ERR_NOSUCHCHANNEL, SplitedMsg);
            if (channels.find(names[i]) == channels.end())
            {
                if (names[i].find_first_of(",\a ") != std::string::npos)
                    throw Myexception(ERR_BADCHANMASK, SplitedMsg);
                ch = new Channel(names[i], keys[i], c);
                if (!keys[i].empty())
                    ch->setMode(MODE_CHANKEY);
                channels[names[i]] = ch;
            }
            else
            {
                ch = channels[names[i]];
                if (ch->getMode() & MODE_CHANKEY && ch->getKey() != keys[i])
                    throw Myexception(ERR_BADCHANNELKEY, SplitedMsg);
                if (ch->getMode() & MODE_USERLIM && ch->getLimit() <= ch->getMembers().size())
                    throw Myexception(ERR_CHANNELISFULL, SplitedMsg);
                bool invited = ch->isInvited(c->getNick());
                if (ch->getMode() & MODE_INVONLY && !invited)
                    throw Myexception(ERR_INVITEONLYCHAN, SplitedMsg);
                ch->addMember(c);
                if (invited)
                    ch->removeInvited(c->getNick());
            }
            std::string msg = c->getIdent() + " JOIN " + names[i] + "\r\n";
            ch->broadcast(msg);
            if (!ch->getTopic().empty())
                c->sendMsg(name + "332 " + c->getNick() + " " + names[i] + " :" + ch->getTopic() + "\r\n");
            c->sendMsg(name + "MODE " + names[i] + " +" + ch->getModeStr() + "\r\n");
            c->sendMsg(name + "353 " + c->getNick() + " = " + names[i] + " :" + ch->getMemberList() + "\r\n");
            c->sendMsg(name + "366 " + c->getNick() + " " + names[i] + " :End of /NAMES list\r\n");
        }
        catch(Myexception &e)
        {
            sendResponce(c->getFd(), this->name
            + int2string(e.getERROR_NO()) + " "
            + c->getNick() + " "
            + SplitedMsg[0] + " "
            + e.what() + "\n");
        }
    }
}

void    Server::cmdlist(std::vector<std::string>& SplitedMsg, Client *c)
{
    if (SplitedMsg.size() == 1)
    {
        for (channelMap::iterator it = channels.begin(); it != channels.end(); it++)
        {
            Channel *ch = it->second;
            std::string msg = name + "322 " + c->getNick() + " " + ch->getName() + " " + int2string(ch->getMembers().size()) + " :";
            msg += ch->getTopic().empty() ? "No topic is set" : ch->getTopic();
            c->sendMsg(msg + "\r\n");
        }
        c->sendMsg(name + "323 " + c->getNick() + " :End of /LIST\r\n");
    }
    else
    {
        std::vector<std::string> names = split(SplitedMsg[1], ',');
        for (size_t i = 0; i < names.size(); i++)
        {
            try
            { 
                if (channels.find(names[i]) == channels.end())
                    throw Myexception(ERR_NOSUCHCHANNEL, SplitedMsg);
                Channel *ch = channels[names[i]];
                std::string msg = name + "322 " + c->getNick() + " " + ch->getName() + " " + int2string(ch->getMembers().size()) + " :";
                msg += ch->getTopic().empty() ? "No topic is set" : ch->getTopic();
                c->sendMsg(msg + "\r\n");
            }
            catch(Myexception &e)
            {
                sendResponce(c->getFd(), this->name
                + int2string(e.getERROR_NO()) + " "
                + c->getNick() + " "
                + SplitedMsg[0] + " "
                + e.what() + "\n");
            }
        }
        c->sendMsg(name + "323 " + c->getNick() + " :End of /LIST\r\n");
    }
}

void	Server::cmdkick(std::vector<std::string>& SplitedMsg, Client *c)
{
    if (SplitedMsg.size() < 3)
        throw Myexception(ERR_NEEDMOREPARAMS, SplitedMsg);
    std::vector<std::string> names = split(SplitedMsg[1], ',');
    std::vector<std::string> targets = split(SplitedMsg[2], ',');
    if (names.size() == 1 && targets.size() > 1)
        names = std::vector<std::string>(targets.size(), names[0]);
    else if (names.size() > 1 && targets.size() == 1)
        targets = std::vector<std::string>(names.size(), targets[0]);
    else if (names.size() != targets.size())
        throw Myexception(ERR_NEEDMOREPARAMS, SplitedMsg);
    for (size_t i = 0; i < names.size(); i++)
    {
        try
        {
            if (names[i][0] != '#')
                throw Myexception(ERR_NOSUCHCHANNEL, SplitedMsg);
            if (channels.find(names[i]) == channels.end())
                throw Myexception(ERR_NOSUCHCHANNEL, SplitedMsg);
            Channel *ch = channels[names[i]];
            if (!ch->isMember(c))
                throw Myexception(ERR_NOTONCHANNEL, SplitedMsg);
            if (!ch->isOperator(c))
                throw Myexception(ERR_CHANOPRIVSNEEDED, SplitedMsg);
            Client *target = getClientByNickname(targets[i]);
            if (!target)
                throw Myexception(ERR_NOSUCHNICK, SplitedMsg);
            if (!ch->isMember(target))
                throw Myexception(ERR_USERNOTINCHANNEL, SplitedMsg);
            std::string msg = c->getIdent() + " KICK " + names[i] + ' ' + target->getNick() + " :";
            msg += SplitedMsg.size() > 3 ? SplitedMsg[3] : "for some reason";
            msg += "\r\n";
            ch->broadcast(msg);
            ch->removeMember(target);
        }
        catch(Myexception &e)
        {
            sendResponce(c->getFd(), this->name
            + int2string(e.getERROR_NO()) + " "
            + c->getNick() + " "
            + SplitedMsg[0] + " "
            + e.what() + "\n");
        }
    }
}

void    Server::cmdpart(std::vector<std::string>& SplitedMsg, Client *c)
{
    if (SplitedMsg.size() < 2)
        throw Myexception(ERR_NEEDMOREPARAMS, SplitedMsg);
    std::vector<std::string> names = split(SplitedMsg[1], ',');
    for (size_t i = 0; i < names.size(); i++)
    {
        try
        {
            if (channels.find(names[i]) == channels.end())
                throw Myexception(ERR_NOSUCHCHANNEL, SplitedMsg);
            Channel *ch = channels[names[i]];
            if (!ch->isMember(c))
                throw Myexception(ERR_NOTONCHANNEL, SplitedMsg);
            std::string msg = c->getIdent() + " PART " + names[i];
            msg += SplitedMsg.size() > 2 ? " :" + SplitedMsg[2] : "";
            msg += "\r\n";
            ch->broadcast(msg);
            ch->removeMember(c);
            if (ch->getMembers().empty())
            {
                delete ch;
                channels.erase(names[i]);
            }
        }
        catch(Myexception &e)
        {
            sendResponce(c->getFd(), this->name
            + int2string(e.getERROR_NO()) + " "
            + c->getNick() + " "
            + SplitedMsg[0] + " "
            + e.what() + "\n");
        }
    }
}

void	Server::cmdinvite(std::vector<std::string>& SplitedMsg, Client *c)
{
    if (SplitedMsg.size() < 3)
        throw Myexception(ERR_NEEDMOREPARAMS, SplitedMsg);
    Client *target = getClientByNickname(SplitedMsg[1]);
    if (target == NULL)
        throw Myexception(ERR_NOSUCHNICK, SplitedMsg);
    if (channels.find(SplitedMsg[2]) == channels.end())
        throw Myexception(ERR_NOSUCHCHANNEL, SplitedMsg);
    Channel *ch = channels[SplitedMsg[2]];
    if (!ch->isMember(c))
        throw Myexception(ERR_NOTONCHANNEL, SplitedMsg);
    if (ch->getMode() & MODE_INVONLY && !ch->isOperator(c))
        throw Myexception(ERR_CHANOPRIVSNEEDED, SplitedMsg);
    if (ch->isMember(target))
        throw Myexception(ERR_USERONCHANNEL, SplitedMsg);
    ch->addInvited(target->getNick());
    std::string msg = c->getIdent() + " INVITE " + target->getNick() + ' ' + SplitedMsg[2] + "\r\n";
    target->sendMsg(msg);
}

void    Server::cmdtopic(std::vector<std::string>& SplitedMsg, Client *c)
{
    if (SplitedMsg.size() < 2)
        throw Myexception(ERR_NEEDMOREPARAMS, SplitedMsg);
    if (channels.find(SplitedMsg[1]) == channels.end())
        throw Myexception(ERR_NOSUCHCHANNEL, SplitedMsg);
    Channel *ch = channels[SplitedMsg[1]];
    if (!ch->isMember(c))
        throw Myexception(ERR_NOTONCHANNEL, SplitedMsg);
    if (SplitedMsg.size() == 2)
    {
        std::string msg = name;
        if (ch->getTopic().empty())
            msg +=  "331 " + c->getNick() + " " + SplitedMsg[1] + " :No topic is set";
        else
            msg +=  "332 " + c->getNick() + " " + SplitedMsg[1] + " :" + ch->getTopic();
        c->sendMsg(msg + "\r\n");
    }
    else
    {
        if (ch->getMode() & MODE_TOPREST && !ch->isOperator(c))
            throw Myexception(ERR_CHANOPRIVSNEEDED, SplitedMsg);
        ch->setTopic(SplitedMsg[2]);
        std::string msg = c->getIdent() + " TOPIC " + SplitedMsg[1] + " :";
        msg += SplitedMsg[2].empty() ? "No topic is set" : SplitedMsg[2];
        ch->broadcast(msg + "\r\n");
    }
}

size_t  strtosize_t(std::string &str)
{
    std::stringstream ss(str);
    int n;
    if (!(ss >> n) || n < 0)
        return (0);
    return (static_cast<size_t>(n));
}

void    Server::cmdmode(std::vector<std::string>& SplitedMsg, Client *c)
{
    if (SplitedMsg.size() < 2)
        throw Myexception(ERR_NEEDMOREPARAMS, SplitedMsg);
    if (channels.find(SplitedMsg[1]) == channels.end())
        throw Myexception(ERR_NOSUCHCHANNEL, SplitedMsg);
    Channel *ch = channels[SplitedMsg[1]];
    if (!ch->isMember(c))
        throw Myexception(ERR_NOTONCHANNEL, SplitedMsg);
    if (SplitedMsg.size() == 2)
    {
        c->sendMsg(name + "324 " + c->getNick() + " " + SplitedMsg[1] + " +" + ch->getModeStr() + "\r\n");
        
    }
    else
    {
        if (!ch->isOperator(c))
            throw Myexception(ERR_CHANOPRIVSNEEDED, SplitedMsg);
        if (SplitedMsg[2] == "+sn")
            return ;
        size_t  arg = 3;
        char    operation = 0;
        std::string seted = "";
        std::string unseted = "";
        std::string unsetedOp = "";
        for (size_t i = 0; i < SplitedMsg[2].size(); i++)
        {
            try
            {
                if (SplitedMsg[2][i] == '-' || SplitedMsg[2][i] == '+')
                {
                    operation = SplitedMsg[2][i];
                    continue ;
                }
                if (operation == '+')
                {
                    if (SplitedMsg[2][i] == 'i')
                    {
                        ch->setMode(MODE_INVONLY);
                        seted += seted.find('i') == std::string::npos ? "i" : "";
                    }
                    else if (SplitedMsg[2][i] == 't')
                    {
                        ch->setMode(MODE_TOPREST);
                        seted += seted.find('t') == std::string::npos ? "t" : "";
                    }
                    else
                    {
                        if (SplitedMsg[2][i] != 'k' && SplitedMsg[2][i] != 'l' && SplitedMsg[2][i] != 'o')
                            throw Myexception(ERR_UNKNOWNCOMMAND, SplitedMsg);
                        if (SplitedMsg.size() < arg + 1)
                            throw Myexception(ERR_NEEDMOREPARAMS, SplitedMsg);
                        if (SplitedMsg[2][i] == 'k')
                        {
                            ch->setKey(SplitedMsg[arg++]);
                            ch->setMode(MODE_CHANKEY);
                            seted += seted.find('k') == std::string::npos ? "k" : "";
                        }
                        else if (SplitedMsg[2][i] == 'l')
                        {
                            size_t l = strtosize_t(SplitedMsg[arg++]);
                            if (!l)
                                continue ;
                            ch->setLimit(l);
                            ch->setMode(MODE_USERLIM);
                            seted += seted.find('l') == std::string::npos ? "l" : "";
                        }
                        else if (SplitedMsg[2][i] == 'o')
                        {
                            Client *target = getClientByNickname(SplitedMsg[arg++]);
                            if (!target)
                                throw Myexception(ERR_NOSUCHNICK, SplitedMsg);
                            if (!ch->isMember(target))
                                throw Myexception(ERR_USERNOTINCHANNEL, SplitedMsg);
                            ch->setOperator(target);
                            seted += seted.find('o') == std::string::npos ? "o" : "";
                        }
                        else
                            throw Myexception(ERR_UNKNOWNCOMMAND, SplitedMsg);
                    }
                }
                else if (operation == '-')
                {
                    if (SplitedMsg[2][i] == 'i')
                    {
                        ch->unsetMode(MODE_INVONLY);
                        unseted += unseted.find('i') == std::string::npos ? "i" : "";
                    }
                    else if (SplitedMsg[2][i] == 't')
                    {
                        ch->unsetMode(MODE_TOPREST);
                        unseted += unseted.find('t') == std::string::npos ? "t" : "";
                    }
                    else if (SplitedMsg[2][i] == 'k')
                    {
                        ch->setKey("");
                        ch->unsetMode(MODE_CHANKEY);
                        unseted += unseted.find('k') == std::string::npos ? "k" : "";
                    }
                    else if (SplitedMsg[2][i] == 'l')
                    {
                        ch->setLimit(0);
                        ch->unsetMode(MODE_USERLIM);
                        unseted += unseted.find('l') == std::string::npos ? "l" : "";
                    }
                    else if (SplitedMsg[2][i] == 'o')
                    {
                        if (SplitedMsg.size() < arg + 1)
                            throw Myexception(ERR_NEEDMOREPARAMS, SplitedMsg);
                        Client *target = getClientByNickname(SplitedMsg[arg++]);
                        if (!target)
                            throw Myexception(ERR_NOSUCHNICK, SplitedMsg);
                        if (!ch->isMember(target))
                            throw Myexception(ERR_USERNOTINCHANNEL, SplitedMsg);
                        ch->unsetOperator(target);
                        unseted += unseted.find('o') == std::string::npos ? "o" : "";
                        unsetedOp = ' ' + target->getNick();
                    }
                    else
                        throw Myexception(ERR_UNKNOWNCOMMAND, SplitedMsg);
                }
                else
                    throw Myexception(ERR_UNKNOWNCOMMAND, SplitedMsg);
            }
            catch(Myexception &e)
            {
                sendResponce(c->getFd(), this->name
                + int2string(e.getERROR_NO()) + " "
                + c->getNick() + " "
                + SplitedMsg[0] + " "
                + e.what() + "\n");
            }
        }
        std::string msg;
        msg += seted.empty() ? "" : " +" + seted;
        arg = 3;
        for (size_t i = 0; i < seted.size(); i++)
        {
            if (seted[i] == 'k' || seted[i] == 'l')
                msg += " " + SplitedMsg[arg++];
            else if (seted[i] == 'o')
                msg += " " + SplitedMsg[arg++];
        }
        msg += unseted.empty() ? "" : " -" + unseted;
        msg += unsetedOp;
        if (!msg.empty())
            ch->broadcast(c->getIdent() + " MODE " + SplitedMsg[1] + msg + "\r\n");
    }
}

void	Server::cmdquit(std::vector<std::string>& SplitedMsg, Client *c)
{
    std::string msg = c->getIdent() + " QUIT : ";
    msg += SplitedMsg.size() > 1 ? SplitedMsg[1] : "Client Quit";
    msg += "\r\n";
    channelMap::iterator it = channels.begin();
    while (it != channels.end())
    {
        Channel *ch = it->second;
        if (ch->isMember(c))
        {
            ch->removeMember(c);
            ch->broadcast(msg);
        }
        if (ch->getMembers().empty())
        {
            delete ch;   
            channels.erase(it++);
        }
        else
            it++;
    }
    clientLeft(c->getFd());
}

void	Server::cmdquit(Client *c, std::string reason)
{
    std::string msg = c->getIdent() + " QUIT : " + reason + "\r\n";
    channelMap::iterator it = channels.begin();
    while (it != channels.end())
    {
        Channel *ch = it->second;
        if (ch->isMember(c))
        {
            ch->removeMember(c);
            ch->broadcast(msg);
        }
        if (ch->getMembers().empty())
        {
            delete ch;   
            channels.erase(it++);
        }
        else
            it++;
    }
    this->clientLeft(c->getFd());
}

void    Server::cmdping(std::vector<std::string>& SplitedMsg, Client *c)
{
    if (SplitedMsg.size() < 2)
        throw Myexception(ERR_NEEDMOREPARAMS, SplitedMsg);
    std::string msg = c->getIdent() + " PONG " + SplitedMsg[1] + "\r\n";
    c->sendMsg(msg);
}

void    Server::cmdpong(std::vector<std::string>& SplitedMsg, Client *c)
{
    if (SplitedMsg.size() < 2)
        throw Myexception(ERR_NEEDMOREPARAMS, SplitedMsg);
    std::string msg = c->getIdent() + " PING " + SplitedMsg[1] + "\r\n";
    c->sendMsg(msg);
}

void    Server::cmdwho(std::vector<std::string>& SplitedMsg, Client *c)
{
    if (SplitedMsg.size() < 2)
        throw Myexception(ERR_NEEDMOREPARAMS, SplitedMsg);
    if (channels.find(SplitedMsg[1]) == channels.end())
        throw Myexception(ERR_NOSUCHCHANNEL, SplitedMsg);
    Channel *ch = channels[SplitedMsg[1]];
    if (!ch->isMember(c))
        throw Myexception(ERR_NOTONCHANNEL, SplitedMsg);
    c->sendMsg(name + "352 " + c->getNick() + " " + SplitedMsg[1] + " " + ch->getMemberList() + "\r\n");
    c->sendMsg(name + "315 " + c->getNick() + " " + SplitedMsg[1] + " :End of /WHO list\r\n");
}