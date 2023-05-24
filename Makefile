# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/16 14:11:30 by tel-bouh          #+#    #+#              #
#    Updated: 2023/05/22 16:04:12 by hasabir          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #






# NAME = webserv
# SRC_DIRS = init acceptConnectionAndGetRequestData configFile init parseConfigFile
# OBJ_DIR = obj

# CC = c++
# CPPFLAGS = -std=c++98


# # OBJSS = $(addprefix $(OBJ_D)/,${SRCS:.cpp=.o})
# OBJS = $(foreach dir,$(SRC_DIRS),$(addprefix $(OBJ_DIR)/,${SRCS:.cpp=.o}))
# # OBJS = $(foreach dir,$(SRC_DIRS),$(patsubst %.cpp,$(OBJ_DIR)/%.o,$(wildcard $(dir)/*.cpp)))
# # OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp)))

# all: $(NAME)

# $(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
# 	$(CC) $(CPPFLAGS) -c $< -o $@

# $(NAME): $(OBJS)
# 	$(CC) $(OBJS) -o $(NAME)
# 	@echo "|  Compilation Done  |"

# $(OBJ_DIR):
# 	mkdir -p $(OBJ_DIR)

# clean:
# 	rm -rf $(OBJ_DIR)

# fclean: clean
# 	rm -f $(NAME)

# re: fclean all


#-------------------------------------------------------------------------

NAME = webserv.out

SRCS = main.cpp \
	   acceptConnectionAndGetRequestData/recieveRequest.cpp \
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
		parseRequest/parseRequest.cpp 
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


#--------------------------------------------------------------------






# NAME = webserv.out

# SRCS = ./main.cpp \
# 	   ./acceptConnectionAndGetRequestData/recieveRequest.cpp \
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
# 	   ./parseRequest.cpp 

# INCLUDES = .

# FLAGS =  -std=c++98

# CC = c++

# all : $(NAME)

# $(NAME) :
# 	$(CC) $(FLAGS) $(SRCS) -I $(INCLUDES) -o $(NAME)

# fclean :
# 	/bin/rm -rf $(NAME)

# re : fclean all
