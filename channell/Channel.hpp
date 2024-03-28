#pragma once

#include <iostream>
#include "../clients/Client.hpp"
#include <map>

#define MODE_INVONLY 0x1
#define MODE_TOPREST 0x2
#define MODE_CHANKEY 0x4
#define MODE_USERLIM 0x8

class Client;

typedef std::map<Client *, bool> MemeberMap;
typedef std::map<int, Client *> ClientMap;

class Channel
{
    std::string const   name;
    uint8_t             mode;
    std::string         key;
    std::string         topic;
    int                 limit;
    MemeberMap          members;
    ClientMap           invited;

    Channel(void);

public:
    Channel(std::string const &name, Client  *op);
    Channel(Channel const & src);
    ~Channel(void);

    Channel & operator=(Channel const & src);

    std::string const   &getName() const;
    uint8_t             getMode() const;
    std::string const   &getKey() const;
    std::string const   &getTopic() const;
    int                 getLimit() const;
    MemeberMap const    &getMembers() const;
    ClientMap const     &getInvited() const;

    void                setKey(std::string const &key);
    void                setTopic(std::string const &topic);
    void                setLimit(int limit);
    void                setMode(uint8_t mode);
    void                unsetMode(uint8_t mode);
    void                addMember(Client *client);
    void                addOperator(Client *client);
};

std::ostream & operator<<(std::ostream & o, Channel const & src);

