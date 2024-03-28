#include "Channel.hpp"

Channel::Channel(std::string const &name, Client  *op) : name(name), mode(0), limit(0)
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

Channel & Channel::operator=(Channel const & src)
{
    if (this != &src)
    {
        // std::cout << "Channel: Copy assignment operator called" << std::endl;
    }
    return *this;
}

std::string const   &Channel::getName() const
{
    return (name);
}

uint8_t             Channel::getMode() const
{
    return (mode);
}

std::string const   &Channel::getKey() const
{
    return (key);
}

std::string const   &Channel::getTopic() const
{
    return (topic);
}

int                 Channel::getLimit() const
{
    return (limit);
}

MemeberMap const    &Channel::getMembers() const
{
    return (members);
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

void                Channel::addOperator(Client *client)
{
    MemeberMap::iterator member = members.find(client);
    if (member != members.end())
        member->second = true;
}

std::ostream & operator<<(std::ostream & o, Channel const & src)
{
    MemeberMap const &members = src.getMembers();
    o << "channel name: " << src.getName();
    o << "channel mode: ";
    o << "i(" << (src.getMode() & MODE_INVONLY ? "true" : "false") << "), ";
    o << "t(" << (src.getMode() & MODE_TOPREST ? "true" : "false") << "), ";
    o << "k(" << (src.getMode() & MODE_CHANKEY ? "true" : "false") << "), ";
    o << "l(" << (src.getMode() & MODE_USERLIM ? "true" : "false") << ")" << std::endl;
    o << "member list :" << std::endl;
    for (MemeberMap::const_iterator i = members.begin(); i != members.end(); i++)
    {
        o << "Client >> " << i->first << " is operator: " << i->second << std::endl;
    }
    
    return o;
}

