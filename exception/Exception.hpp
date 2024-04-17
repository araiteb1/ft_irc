/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 10:27:41 by araiteb           #+#    #+#             */
/*   Updated: 2024/04/17 21:59:17 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  EXCEPTION_HPP
# define EXCEPTION_HPP

#include<iostream>
#include<string>
#include<vector>

enum e_rronum{
    ERR_NOTREGISTERED = 451,
    ERR_NEEDMOREPARAMS = 461,
    ERR_ALREADYREGISTRED = 462,
    
    ERR_NONICKNAMEGIVEN = 431,
    ERR_ERRONEUSNICKNAME = 432,
    ERR_NICKNAMEINUSE = 433,
    ERR_NICKCOLLISION = 436,


    ERR_NORECIPIENT = 411,
    ERR_CANNOTSENDTOCHAN = 404,
    ERR_WILDTOPLEVEL = 414,
    ERR_NOSUCHNICK = 401,
    ERR_NOTEXTTOSEND = 412,
    ERR_NOTOPLEVEL = 413,
    ERR_TOOMANYTARGETS = 407,
    ERR_PASSWDMISMATCH = 464,
    ERR_UNKNOWNCOMMAND = 421,
    ERR_NOSUCHCHANNEL = 403,
    ERR_CHANNELISFULL = 471,
    ERR_INVITEONLYCHAN = 473,
    ERR_BADCHANNELKEY = 475,
    ERR_BADCHANMASK = 476,
    ERR_CHANOPRIVSNEEDED = 482,
    ERR_USERNOTINCHANNEL = 441,
    ERR_NOTONCHANNEL = 442,
    ERR_USERONCHANNEL = 443
};

enum  e_repl{
    RPL_AWAY = 301
};

class Myexception : public std::exception{
         private:
            int err_num;
            std::string err_msg;
        public:
            Myexception(std::string msg) : err_msg(msg) {}
            Myexception(int err_number, std::vector< std::string>& SplitMsg) {
                err_num = err_number;
                err_msg = msgError(err_num, SplitMsg);
            }
            virtual ~Myexception() throw() {}
            

            std::string msgError(int num , std::vector< std::string> &SplitMsg);
            char const *    what() const throw() {
                return (err_msg.c_str());
            }
            int                getERROR_NO() {
                return (err_num);
            }
};
#endif

