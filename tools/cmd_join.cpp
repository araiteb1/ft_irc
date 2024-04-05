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
        keys.insert(keys.begin(), tmp.begin(), tmp.end());
    }
    for (size_t i = 0; i < names.size(); i++)
    {
        Channel *ch;
        if (names[i][0] != '#')
            throw Myexception(ERR_NOSUCHCHANNEL);
        if (channels.find(names[i]) == channels.end())
        {
            ch = new Channel(names[i], keys[i], c);
            channels[names[i]] = ch;
        }
        else
        {
            ch = channels[names[i]];
            if (ch->getKey() != keys[i])
                throw Myexception(ERR_BADCHANNELKEY);
            if (ch->getMode() & MODE_USERLIM && ch->getLimit() <= ch->getMembers().size())
                throw Myexception(ERR_CHANNELISFULL);
            if (ch->getMode() & MODE_INVONLY && !ch->isInvited(c))
                throw Myexception(ERR_INVITEONLYCHAN);
            ch->addMember(c);
        }
        ch->broadcast(':' + name + ' ' + c->getNick() + " JOIN " + names[i] + "\f\r", c);
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
        ch->broadcast(msg, c);
        ch->removeMember(target);
    }
}


