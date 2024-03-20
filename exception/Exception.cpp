/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 10:27:37 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/20 11:13:06 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Exception.hpp"

std::string     Myexception::msgError(int num)
{
    switch (num){
        case ERR_NEEDMOREPARAMS:
            return (":Not enough parameters");
        case ERR_ALREADYREGISTRED:
            return (":You may not reregister");
        case ERR_NONICKNAMEGIVEN:
            return (":No nickname given");
        case ERR_ERRONEUSNICKNAME:
            return (":Erroneus nickname");
        case ERR_NICKNAMEINUSE:
            return (":Nickname is already in use");
        case ERR_NICKCOLLISION:
            return (":Nickname collision KILL");
        case ERR_NORECIPIENT:
            return (":No recipient given (<command>)");
        case ERR_CANNOTSENDTOCHAN:
            return (":Cannot send to channel");
        case ERR_WILDTOPLEVEL:
            return (":Wildcard in toplevel domain");
        case ERR_NOSUCHNICK:
            return (":No such nick/channel");
        case ERR_NOTEXTTOSEND:
            return (":No text to send");
        case ERR_NOTOPLEVEL:
            return (":No toplevel domain specified");
        case ERR_TOOMANYTARGETS:
            return (":Duplicate recipients. No message delivered");
        case ERR_PASSWDMISMATCH:
            return (":Password incorrect");
        case ERR_UNKNOWNCOMMAND:
            return (":Unknown command");
        case ERR_NOTREGISTERED:
            return (":You have not registered");
        default:
            return(":Undefined reply");
    }
}