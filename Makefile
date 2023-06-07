# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/16 14:11:30 by tel-bouh          #+#    #+#              #
#    Updated: 2023/06/07 16:52:32 by hasabir          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRCS = ./main.cpp \
	   acceptConnectionAndGetRequestData/recieveRequest.cpp \
	   acceptConnectionAndGetRequestData/splitBody.cpp \
	   acceptConnectionAndGetRequestData/endOfRequest.cpp \
	   acceptConnectionAndGetRequestData/handleConnection.cpp  \
	   init/displayHostPort.cpp ./init/initServer.cpp \
	   init/initWebStructAndFree.cpp ./init/activeSocket.cpp\
	   parseConfigFile/parseConfigFile.cpp \
	   parseConfigFile/getConfigData.cpp \
	   parseConfigFile/parseConfigFun1.cpp \
	   parseConfigFile/moveBracketsToNextLine.cpp \
	   parseConfigFile/valideServersBlock.cpp \
	   parseConfigFile/displayServerFile.cpp \
	   parseConfigFile/checkConfigdata.cpp \
	   parseConfigFile/splitSemiColons.cpp \
	   parseConfigFile/valides.cpp  \
	   parseRequest/parseRequest.cpp client.cpp\
	   parseRequest/get.cpp parseRequest/post.cpp\
	   parseRequest/deleteResponse.cpp\
	   parseRequest/utils.cpp parseRequest/parsRequestHeader.cpp\
	   parseRequest/sendResponse.cpp parseRequest/getResponseData.cpp
# parseRequest.cpp

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















# NAME = webserv.out

# SRCS = ./main.cpp \
# 	   ./acceptConnectionAndGetRequestData/recieveRequest.cpp \
# 	   ./acceptConnectionAndGetRequestData/splitBody.cpp \
# 	   ./acceptConnectionAndGetRequestData/endOfRequest.cpp \
# 	   ./acceptConnectionAndGetRequestData/handleConnection.cpp  \
# 	   ./init/displayHostPort.cpp ./init/initServer.cpp \
# 	   ./init/initWebStructAndFree.cpp ./init/activeSocket.cpp\
# 	   ./parseConfigFile/parseConfigFile.cpp \
# 	   ./parseConfigFile/getConfigData.cpp \
# 	   ./parseConfigFile/parseConfigFun1.cpp \
# 	   ./parseConfigFile/moveBracketsToNextLine.cpp \
# 	   ./parseConfigFile/valideServersBlock.cpp \
# 	   ./parseConfigFile/displayServerFile.cpp \
# 	   ./parseConfigFile/checkConfigData.cpp \
# 	   ./parseConfigFile/splitSemiColons.cpp \
# 	   ./parseConfigFile/valides.cpp  \
# 	   ./parseRequest.cpp ./client.cpp

# INCLUDES = .

# FLAGS =  -std=c++98

# CC = c++

# all : $(NAME)

# $(NAME) :
# 	$(CC) $(FLAGS) $(SRCS) -I $(INCLUDES) -o $(NAME)

# fclean :
# 	/bin/rm -rf $(NAME)

# re : fclean all
