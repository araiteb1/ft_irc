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
	invited.insert(src.getInvited().begin(), src.getInvited().end());
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

std::string const   &Channel::getModeStr() const
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

ClientMap const     &Channel::getInvited() const
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

bool                Channel::isInvited(Client *client) const
{
	return (invited.find(client->getFd()) != invited.end());
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

void				Channel::addInvited(Client *client)
{
	invited.insert(std::make_pair(client->getFd(), client));
}

void				Channel::removeMember(Client *client)
{
	MemberMap::iterator member = members.find(client);
	if (member != members.end())
		members.erase(member);
}

void                Channel::setOperator(Client *client)
{
	MemberMap::iterator member = members.find(client);
	if (member != members.end())
		member->second = true;
}

void				Channel::broadcast(std::string const &msg, Client *sender)
{
	for (MemberMap::iterator i = members.begin(); i != members.end(); i++)
		if (i->first != sender) i->first->sendMsg(msg);
}

std::ostream & operator<<(std::ostream & o, Channel const & src)
{
	MemberMap const &members = src.getMembers();
	o << "channel name: " << src.getName();
	o << "channel mode: ";
	o << "i(" << (src.getMode() & MODE_INVONLY ? "true" : "false") << "), ";
	o << "t(" << (src.getMode() & MODE_TOPREST ? "true" : "false") << "), ";
	o << "k(" << (src.getMode() & MODE_CHANKEY ? "true" : "false") << "), ";
	o << "l(" << (src.getMode() & MODE_USERLIM ? "true" : "false") << ")" << std::endl;
	o << "member list :" << std::endl;
	for (MemberMap::const_iterator i = members.begin(); i != members.end(); i++)
	{
		o << "Client >> " << i->first << " is operator: " << i->second << std::endl;
	}
	
	return o;
}

