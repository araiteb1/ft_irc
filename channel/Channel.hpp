#pragma once

#include <iostream>
#include "../clients/Client.hpp"
#include <map>
#include <algorithm>

#define MODE_INVONLY 0x1
#define MODE_TOPREST 0x2
#define MODE_CHANKEY 0x4
#define MODE_USERLIM 0x8

class Client;

typedef std::map<Client *, bool> MemberMap;
typedef std::map<int, Client *> ClientMap;

class Channel
{
    std::string const   name;
    uint8_t             mode;
    std::string         key;
    std::string         topic;
    size_t             limit;
    MemberMap           members;
    ClientMap           invited;

    Channel(void);
    Channel & operator=(Channel const & src);

public:
    Channel(std::string const &name, std::string const &key , Client  *op);
    Channel(Channel const & src);
    ~Channel(void);


    std::string const   &getName() const;
    uint8_t             getMode() const;
    std::string const   &getKey() const;
    std::string const   &getTopic() const;
    size_t              getLimit() const;
    MemberMap const     &getMembers() const;
    ClientMap const     &getInvited() const;
    std::string         getMemberList() const;
    bool                isMember(Client *client) const;
    bool                isInvited(Client *client) const;
    bool                isOperator(Client *client) const;

    void                setKey(std::string const &key);
    void                setTopic(std::string const &topic);
    void                setLimit(int limit);
    void                setMode(uint8_t mode);
    void                unsetMode(uint8_t mode);
    void                addMember(Client *client);
    void                addInvited(Client *client);
    void                removeMember(Client *client);
    void                setOperator(Client *client);

    void                broadcast(std::string const &msg, Client *sender);
};

std::ostream & operator<<(std::ostream & o, Channel const & src);

