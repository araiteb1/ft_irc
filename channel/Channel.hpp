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
typedef std::vector<std::string> InviteList;

class Channel
{
    std::string const   name;
    uint8_t             mode;
    std::string         key;
    std::string         topic;
    size_t              limit;
    MemberMap           members;
    InviteList          invited;

    Channel(void);
    Channel & operator=(Channel const & src);

public:
    Channel(std::string const &name, std::string const &key , Client  *op);
    Channel(Channel const & src);
    ~Channel(void);


    std::string const   &getName() const;
    uint8_t             getMode() const;
    std::string const   getModeStr() const;
    std::string const   &getKey() const;
    std::string const   &getTopic() const;
    size_t              getLimit() const;
    MemberMap const     &getMembers() const;
    InviteList const    &getInvited() const;
    std::string         getMemberList() const;
    bool                isMember(Client *client) const;
    bool                isInvited(std::string const &nick) const;
    bool                isOperator(Client *client) const;

    void                setKey(std::string const &key);
    void                setTopic(std::string const &topic);
    void                setLimit(int limit);
    void                setMode(uint8_t mode);
    void                unsetMode(uint8_t mode);
    void                addMember(Client *client);
    void                addInvited(std::string const &nick);
    void                removeMember(Client *client);
    void                removeInvited(std::string const &nick);
    void                setOperator(Client *client);
    void                unsetOperator(Client *client);

    void                broadcast(std::string const &msg);
};

std::ostream & operator<<(std::ostream & o, Channel const & src);

