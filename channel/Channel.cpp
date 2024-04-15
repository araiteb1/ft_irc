#include "Channel.hpp"

Channel::Channel(std::string const &name, std::string const &key , Client  *op)
	: name(name), mode(0), key(key), limit(0)
{
	// std::cout << "Channel: Default constructor called" << std::endl;
	members.insert(std::make_pair(op, true));
	return ; 
}

Channel::Channel(Channel const & src)
: name(src.getName()), mode(src.getMode()), key(src.getKey()), topic(src.getTopic()), limit(src.getLimit())
{
	// std::cout << "Channel: Copy constructor called" << std::endl;
	members.insert(src.getMembers().begin(), src.getMembers().end());
	invited.assign(src.getInvited().begin(), src.getInvited().end());
	return ;
}

Channel::~Channel(void)
{
	// std::cout << "Channel: Destructor called" << std::endl;
	return ;
}

std::string const   &Channel::getName() const
{
	return (name);
}

uint8_t             Channel::getMode() const
{
	return (mode);
}

std::string const   Channel::getModeStr() const
{
	std::string modeStr;
	if (mode & MODE_INVONLY)
		modeStr += "i";
	if (mode & MODE_TOPREST)
		modeStr += "t";
	if (mode & MODE_CHANKEY)
		modeStr += "k";
	if (mode & MODE_USERLIM)
		modeStr += "l";
	if (mode & MODE_CHANKEY)
		modeStr += ' ' + key;
	if (mode & MODE_USERLIM)
		modeStr += ' ' + limit;
	return (modeStr);
}

std::string const   &Channel::getKey() const
{
	return (key);
}

std::string const   &Channel::getTopic() const
{
	return (topic);
}

size_t              Channel::getLimit() const
{
	return (limit);
}

MemberMap const     &Channel::getMembers() const
{
	return (members);
}

InviteList const    &Channel::getInvited() const
{
	return (invited);
}

bool				Channel::isOperator(Client *client) const
{
	MemberMap::const_iterator i = members.find(client);
	if (i != members.end())
		return (i->second);
	return (false);
}

std::string			Channel::getMemberList() const
{
	std::string list;
	MemberMap::const_iterator i = members.begin();
	while (i != members.end())
	{
		if (i->second)
			list += "@";
		list += i->first->getNick();
		if (++i != members.end())
			list += " ";
	}
	return (list);
}

bool                Channel::isMember(Client *client) const
{
	return (members.find(client) != members.end());
}

bool                Channel::isInvited(std::string const &nick) const
{
	return (std::find(invited.begin(), invited.end(), nick) != invited.end());
}

void                Channel::setKey(std::string const &key)
{
	this->key = key;
}

void                Channel::setTopic(std::string const &topic)
{
	this->topic = topic;
}

void                Channel::setLimit(int limit)
{
	this->limit = limit;
}

void                Channel::setMode(uint8_t mode)
{
	this->mode |= mode;
}

void                Channel::unsetMode(uint8_t mode)
{
	if (this->mode & mode)
		this->mode &= ~mode;
}

void                Channel::addMember(Client *client)
{
	if (members.find(client) == members.end())
		members.insert(std::make_pair(client, false));
}

void				Channel::addInvited(std::string const &nick)
{
	invited.push_back(nick);
}

void				Channel::removeMember(Client *client)
{
	MemberMap::iterator member = members.find(client);
	if (member != members.end())
		members.erase(member);
}

void				Channel::removeInvited(std::string const &nick)
{
	InviteList::iterator i = std::find(invited.begin(), invited.end(), nick);
	if (i != invited.end())
		invited.erase(i);
}

void                Channel::setOperator(Client *client)
{
	MemberMap::iterator member = members.find(client);
	if (member != members.end())
		member->second = true;
}

void				Channel::unsetOperator(Client *client)
{
	MemberMap::iterator member = members.find(client);
	if (member != members.end())
		member->second = false;
}

void				Channel::broadcast(std::string const &msg)
{
	for (MemberMap::iterator i = members.begin(); i != members.end(); i++)
		i->first->sendMsg(msg);
}

void                Channel::broadcast(std::string const &msg, Client *sender)
{
		for (MemberMap::iterator i = members.begin(); i != members.end(); i++)
		{
			if (i->first != sender)
				i->first->sendMsg(msg);
		}
}

std::ostream & operator<<(std::ostream & o, Channel const & src)
{
	o << "channel name: " << src.getName();
	o << "channel mode: " << src.getModeStr() << std::endl;
	o << "member list :" << src.getMemberList() << std::endl;
	return o;
}

