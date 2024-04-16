/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 12:11:12 by araiteb           #+#    #+#             */
/*   Updated: 2024/04/16 18:47:18 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"

int	split(std::string msg, std::vector<std::string> &SplitedMsg)
{
	std::cout << "recv  [" << msg << "]" << std::endl;
	std::vector<std::string> tmpCommand;
	std::string tmp;
	std::string tmp2;
	int len = msg.size();
	int pos2poin = msg.find_first_of(":");
	if (pos2poin == -1)
		pos2poin = len;
	tmp = msg.substr(0, pos2poin);
	if (!tmp.empty())
		tmpCommand.push_back(tmp);
	if ( pos2poin < len)
		tmp2= msg.substr(pos2poin + 1, len);
	if (!tmp2.empty())
		tmpCommand.push_back(tmp2);
	if (!tmpCommand.size())
		return 0;
	splitCommand(tmpCommand[0], ' ', SplitedMsg);
	if (tmpCommand.size() > 1 && !tmpCommand[1].empty())
		SplitedMsg.push_back(tmpCommand[1]);
	tmpCommand.clear();
	return 1;

}

void    splitCommand(std::string str, char oper, std::vector<std::string> &SplitedMsg)
{
	int ind = 0;
	int i = 0;
	int start = 0;
	int end = 0;
	int len = str.length();


	while(i <= len)
	{
		if ((str[i] == oper) || (i == len))
		{
			end = i;
			std::string subStr = "";
			subStr.append(str, start, end - start);
			if (!subStr.empty())
			SplitedMsg.push_back(subStr);
			ind++;
			start = end + 1;
		}
		i++;
	}
}

std::string const   int2string(int n) {
    std::stringstream    ss;

    ss << n;
    return (ss.str());
}