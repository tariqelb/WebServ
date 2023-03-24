# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/16 14:11:30 by tel-bouh          #+#    #+#              #
#    Updated: 2023/03/24 15:31:47 by tel-bouh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv.out

SRCS = ./main.cpp ./handleRequest.cpp ./handleConnection.cpp ./parseRequest.cpp \
	   ./parseRequestData.cpp ./displayHostPort.cpp ./initServer.cpp ./split.cpp \
	   ./activeSocket.cpp ./parseConfigFile.cpp ./checkForSemiColon.cpp \
	   ./getConfigData.cpp ./parseConfigFun1.cpp ./parseConfigFun2.cpp \
	   ./splitSemiColons.cpp ./valideServersBlock.cpp ./valideDirectiveName.cpp \
	   ./valides.cpp 

INCLUDES = .

FLAGS =  -std=c++98

CC = c++

all : $(NAME)

$(NAME) :
	$(CC) $(FLAGS) $(SRCS) -I $(INCLUDES) -o $(NAME)

fclean :
	/bin/rm -rf $(NAME)

re : fclean all
