/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 10:24:34 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/20 11:15:25 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

Message::Message(){
    
}
int     checkspace(std::string s)
{
    int len = s.length();
    for(int i = 0; i < len ; i++)
    {
        if (s[i] != ' ')
            return 1;
    }
    return 0;
}
Message::Message(int fd, std::string msg){
    char *ptr;
    std::string tmp;

    this->SenderFd = fd;
    ptr = strtok((char *)msg.c_str(),"\r\n");
    while (ptr != NULL)
    {
        this->full_msg.push_back(ptr);
        ptr = strtok(NULL,"\r\n");
    }
    if (full_msg.size() > 0 &&  checkspace(full_msg[0]) == 1)
        this->message = full_msg[0];
    else
        this->message = "";
}

Message::~Message(){

}
void    Message::setMessage(std::string msg){
    this->message = msg;
}
std::string     Message::getMessage(){
    return this->message;
}

int         Message::getSenderFd() 
{
    return (this->SenderFd);
}

void        Message::setSenderFd(int fd)
{
    this->SenderFd = fd;
}