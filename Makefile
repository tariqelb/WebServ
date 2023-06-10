# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/16 14:11:30 by tel-bouh          #+#    #+#              #
#    Updated: 2023/06/09 17:33:23 by tel-bouh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv.out

SRCS = 	main.cpp \
		acceptConnectionAndGetRequestData/endOfRequest.cpp\
		acceptConnectionAndGetRequestData/handleConnection.cpp\
		acceptConnectionAndGetRequestData/multiTypes.cpp\
		acceptConnectionAndGetRequestData/recieveRequest.cpp\
		acceptConnectionAndGetRequestData/splitBody.cpp\
		acceptConnectionAndGetRequestData/getFilesLength.cpp\
		init/activeSocket.cpp init/displayHostPort.cpp\
		init/initServer.cpp init/initWebStructAndFree.cpp\
		parseConfigFile/checkConfigdata.cpp parseConfigFile/displayServerFile.cpp\
		parseConfigFile/getConfigData.cpp\
		parseConfigFile/moveBracketsToNextLine.cpp\
		parseConfigFile/parseConfigFile.cpp parseConfigFile/parseConfigFun1.cpp\
		parseConfigFile/splitSemiColons.cpp parseConfigFile/valideServersBlock.cpp\
		parseConfigFile/valides.cpp parseRequest/deleteResponse.cpp\
		parseRequest/get.cpp parseRequest/parsRequestHeader.cpp\
		parseRequest/parseRequest.cpp parseRequest/post.cpp\
		parseRequest/sendResponse.cpp parseRequest/utils.cpp\
		client.cpp\

OBJS = ${SRCS:.cpp=.o}

CC = c++
FLAGS = -std=c++98

all : ${NAME}

%.o: %.cpp
	@$(CC) ${FLAGS} -c -o $@ $<
  
${NAME} :${OBJS}
	@$(CC) $(OBJS) -o $(NAME)
	@echo "\033[92m|  Compilation Done  |\033[00m"

clean:
	@rm -rf ${OBJS}
	@echo "|  CLEANED |"

fclean: clean
	@rm -rf ${NAME}

re: fclean all


# INCLUDES = .

# FLAGS =  -std=c++98

# CC = c++

# all : $(NAME)

# $(NAME) :
# 	$(CC) $(FLAGS) $(SRCS) -I $(INCLUDES) -o $(NAME)

# fclean :
# 	/bin/rm -rf $(NAME)

# re : fclean all
