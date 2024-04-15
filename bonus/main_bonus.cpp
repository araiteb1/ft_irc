/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 14:08:00 by araiteb           #+#    #+#             */
/*   Updated: 2024/04/15 14:53:10 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

void 	Bot::traitResvedData(std::string &msg, int client_fd) {
	std::vector<std::string> SplitedMsg;
    char message[1548];
	std::string sender;
	std::string arg;
    std::string command;
    std::vector<std::string> tmp1;
    std::vector<std::string> tmp2;
    std::cout << msg;
    splitCommand (msg, ':', tmp1);
    command = tmp1[1];
    command = command.substr(0, command.size()-2); 
    splitCommand (tmp1[0], '!', tmp2);
    sender = tmp2[0];
    if (!command.empty() && !command.compare("time")) {
        sprintf(message, "privmsg %s :%s\r\n", sender.c_str(), returntime().c_str());
        sendResponce(client_fd, message);
    }
    else if (!command.empty() && !command.compare("help")){
        sendResponce(client_fd , "privmsg " +sender+ ": * Available commands: \n -Command: PASS / Parameters: <password> \n");
        usleep(1);
        sendResponce(client_fd , "privmsg " +sender+ ": -Command: PASS / Parameters: <password> \n");
        usleep(1);
        sendResponce(client_fd , "privmsg " +sender+ ": -Command: NICK / Parameters: <nickname> [ <hopcount> ] \n");
        usleep(1);
        sendResponce(client_fd , "privmsg " +sender+ ": -Command: USER / Parameters: <username> <hostname> <servername> <realname> \n");
        usleep(1);
        sendResponce(client_fd , "privmsg " +sender+ ": -Command: JOIN / Parameters: <channel>{,<channel>} [<key>{,<key>}] \n");
        usleep(1);
        sendResponce(client_fd , "privmsg " +sender+ ": -Command: TOPIC / Parameters: <channel> [<topic>] \n");
        usleep(1);
        sendResponce(client_fd , "privmsg " +sender+ ": -Command: INVITE / Parameters: <nickname> <channel> \n");
        usleep(1);
        sendResponce(client_fd , "privmsg " +sender+ ": -Command: KICK  / Parameters: <channel> <user> [<comment>] \n");
        usleep(1);
        sendResponce(client_fd , "privmsg " +sender+ ": -Command: PRIVMSG / Parameters: <receiver>{,<receiver>} <text to be sent> \n");
        usleep(1);
    }
    tmp1.clear();
    tmp2.clear();
}


int main(int ac, char **av){
    
    if(ac != 3){
        std::cerr << "prob in arguments" << std::endl;
        return 0;
    }
    struct sockaddr_in serv_addr;
    int port = atoi(av[1]);
    std::string pass = av[2];
    Bot *betbot = new Bot();
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Socket creation failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
 
    if ((connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        std::cout << "Connection Failed  " << std::endl;;
        return -1;
    }
    betbot->ConnetToServer(sockfd, pass);
    delete betbot;
    close(sockfd); 
}
