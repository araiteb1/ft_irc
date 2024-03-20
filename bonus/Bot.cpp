/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 14:08:27 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/20 14:09:21 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

Bot::Bot()
{
    this->name = "BetBot";
    this->nickname = "Bot";
    this->username = "betbot1";
    this->fullname = "dd";
    this->servername = "localhost";
    this->hostname = "localhost";
}

Bot::~Bot() {}

Bot& Bot::operator=(Bot &b)
{
    this->name = b.name;
    this->nickname = b.nickname;
    this->username = b.username;
    this->fullname = b.fullname;
    this->servername = b.servername;
    this->hostname = b.hostname;
    return *this;
}

Bot::Bot(Bot &b) { *this = b; }

void    Bot::setStudent_13(std::map<std::string, float> Student_13) { this->Student_13 = Student_13;}

std::map<std::string, float> Bot::getStudent_13() { return (this->Student_13);}

void Bot::ConnetToServer(int fd, std::string &pass)
{
    char message[1024];
    sleep(1);
    sprintf(message, "PASS %s\r\n", pass.c_str());
    send(fd, message, strlen(message), 0);
    sleep(1);
    sprintf(message, "USER %s * * :%s\r\n", this->username.c_str(), this->fullname.c_str());
    send(fd, message, strlen(message), 0);
    sleep(1);
    sprintf(message, "NICK %s\r\n", this->nickname.c_str());
    send(fd, message, strlen(message), 0);
    sleep(1);
    while (true) {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        int rc = recv(fd, buffer, sizeof(buffer), 0);
        if (rc <= 0) {
            std::cerr << "Receive failed" << std::endl;
            break;
        }
        std::string message(buffer, rc);
        this->traitResvedData(message, fd);
    }
}
