/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anammal <anammal@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 10:27:37 by araiteb           #+#    #+#             */
/*   Updated: 2024/04/14 16:45:43 by anammal          ###   ########.fr       */
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
        case ERR_NOSUCHCHANNEL:
            return (":No such channel");
        case ERR_CHANNELISFULL:
            return (":Cannot join channel (+l)");
        case ERR_INVITEONLYCHAN:
            return (":Cannot join channel (+i)");
        case ERR_BADCHANNELKEY:
            return (":Cannot join channel (+k)");
        case ERR_BADCHANMASK:
            return (":Bad Channel Mask");
        case ERR_CHANOPRIVSNEEDED:
            return (":You're not channel operator");
        case ERR_USERNOTINCHANNEL:
            return (":They aren't on that channel");
        case ERR_NOTONCHANNEL:
            return (":You're not on that channel");
        case ERR_USERONCHANNEL:
            return (":is already on channel");
        default:
            return(":Undefined reply");
    }
}
