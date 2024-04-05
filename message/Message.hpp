/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 10:24:31 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/19 11:57:39 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <vector>

class Message
{
    private:
        std::vector<std::string> full_msg;
        std::string message;
        int         SenderFd;
    
    public:
        Message();
        Message(int, std::string );
        ~Message();


        std::string getMessage();
        int         getSenderFd();

        void        setMessage(std::string );
        void        setSenderFd(int);
};

#endif