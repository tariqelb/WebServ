# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/16 14:11:30 by tel-bouh          #+#    #+#              #
#    Updated: 2023/06/06 20:21:40 by tel-bouh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv.out

SRCS = ./main.cpp \
	   ./acceptConnectionAndGetRequestData/recieveRequest.cpp \
	   ./acceptConnectionAndGetRequestData/splitBody.cpp \
	   ./acceptConnectionAndGetRequestData/multiTypes.cpp \
	   ./acceptConnectionAndGetRequestData/endOfRequest.cpp \
	   ./acceptConnectionAndGetRequestData/handleConnection.cpp  \
	   ./init/displayHostPort.cpp ./init/initServer.cpp \
	   ./init/initWebStructAndFree.cpp ./init/activeSocket.cpp\
	   ./parseConfigFile/parseConfigFile.cpp \
	   ./parseConfigFile/getConfigData.cpp \
	   ./parseConfigFile/parseConfigFun1.cpp \
	   ./parseConfigFile/moveBracketsToNextLine.cpp \
	   ./parseConfigFile/valideServersBlock.cpp \
	   ./parseConfigFile/displayServerFile.cpp \
	   ./parseConfigFile/checkConfigData.cpp \
	   ./parseConfigFile/splitSemiColons.cpp \
	   ./parseConfigFile/valides.cpp  \
	   ./client.cpp

INCLUDES = .

FLAGS =  -std=c++98

CC = c++

all : $(NAME)

$(NAME) :
	$(CC) $(FLAGS) $(SRCS) -I $(INCLUDES) -o $(NAME)

fclean :
	/bin/rm -rf $(NAME)

re : fclean all
