# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/16 14:11:30 by tel-bouh          #+#    #+#              #
#    Updated: 2023/03/18 11:45:39 by tel-bouh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv.out

SRCS = ./main.cpp ./handleRequest.cpp ./handleConnection.cpp ./parseRequest.cpp \
	   ./parseRequestData.cpp ./displayHostPort.cpp ./initServer.cpp ./split.cpp \
	   ./activeSocket.cpp ./parseConfigFile.cpp

INCLUDES = .

FLAGS =  -std=c++98

CC = c++

all : $(NAME)

$(NAME) :
	$(CC) $(FLAGS) $(SRCS) -I $(INCLUDES) -o $(NAME)

fclean :
	/bin/rm -rf $(NAME)

re : fclean all
