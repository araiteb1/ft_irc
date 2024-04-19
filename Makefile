# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anammal <anammal@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/19 12:15:46 by araiteb           #+#    #+#              #
#    Updated: 2024/04/19 14:52:48 by anammal          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

EXEC = ircserv
EXECB = Bot
INCLUDE = ./server/Server.hpp ./clients/Client.hpp ./message/Message.hpp  ./exception/Exception.hpp
INCLUDEB = ./bonus/Bot.hpp ./clients/Client.hpp ./message/Message.hpp  ./exception/Exception.hpp
SRCS = main.cpp ./clients/Client.cpp ./server/Server.cpp ./tools/ft_split.cpp  ./message/Message.cpp ./exception/Exception.cpp \
	./tools/commands.cpp ./channel/Channel.cpp
SRCSB = ./bonus/main_bonus.cpp ./bonus/Bot.cpp ./bonus/Bot_commands.cpp ./tools/ft_split.cpp  ./message/Message.cpp

CC = c++ 
CFLAGS = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address
OBJS = $(SRCS:.cpp=.o)
OBJSB = $(SRCSB:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(EXECB): $(OBJSB)
	$(CC) $(CFLAGS) $^ -o $@
%.o: %.cpp $(INCLUDE)
	$(CC) $(CFLAGS) -c  $< -o $@

bonus: $(OBJSB)
	@$(CC) $(FLAGS) -o $(EXECB) $(OBJSB)

clean:
	rm -f $(OBJS) $(OBJSB)

fclean: clean
	rm -f $(EXEC) $(EXECB)

re : fclean all
.PHONY: all clean fclean