#include "../server/Server.hpp"
#include "../channel/Channel.hpp"
#include <sstream>

std::vector<std::string> split(std::string str, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delim))
        result.push_back(token);
    return result;
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
        ch->broadcast(msg, c);
        if (!ch->getTopic().empty())
            c->sendMsg(":" + name + " 332 " + c->getNick() + " " + names[i] + " :" + ch->getTopic() + "\r\n");
        c->sendMsg(":" + name + " 353 " + c->getNick() + " = " + names[i] + " :" + ch->getMemberList() + "\r\n");
        c->sendMsg(":" + name + " 366 " + c->getNick() + " " + names[i] + " :End of /NAMES list\r\n");
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
        ch->broadcast(msg, c);
        ch->removeMember(target);
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
        std::string msg = ":" + name + " 331 " + c->getNick() + " " + SplitedMsg[1] + " :";
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
        ch->broadcast(msg, c);
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
    {
        c->sendMsg(":" + name + " 324 " + c->getNick() + " " + SplitedMsg[1] + " +" + ch->getModeStr() + "\r\n");
    }
    else
    {
        if (!ch->isOperator(c))
            throw Myexception(ERR_CHANOPRIVSNEEDED);
        if (SplitedMsg[2][0] == '+')
        {
            int arg = 3;
            for (size_t i = 1; i < SplitedMsg[2].size(); i++)
            {
                if (SplitedMsg[2][i] == 'i')
                    ch->setMode(MODE_INVONLY);
                else if (SplitedMsg[2][i] == 't')
                    ch->setMode(MODE_TOPREST);
                else if (SplitedMsg[2][i] == 'k')
                {
                    if (SplitedMsg.size() < arg + 1)
                        throw Myexception(ERR_NEEDMOREPARAMS);
                    ch->setKey(SplitedMsg[arg++]);
                    ch->setMode(MODE_CHANKEY);
                }
                else if (SplitedMsg[2][i] == 'l')
                {
                    if (SplitedMsg.size() < arg + 1)
                        throw Myexception(ERR_NEEDMOREPARAMS);
                    ch->setLimit(std::atoi(SplitedMsg[arg++].c_str()));
                    ch->setMode(MODE_USERLIM);
                }
            }
        }
        else if (SplitedMsg[2][0] == '-')
        {
            for (size_t i = 1; i < SplitedMsg[2].size(); i++)
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
                else
                    throw Myexception(ERR_UNKNOWNCOMMAND);
            }
        }
        else
            throw Myexception(ERR_UNKNOWNCOMMAND);
        std::string msg = ':' + c->getNick() + '!' + c->getusername() + '@' + c->gethostname() + " MODE " + SplitedMsg[1] + ' ' + SplitedMsg[2] + "\r\n";
        ch->broadcast(msg, c);
    }
}