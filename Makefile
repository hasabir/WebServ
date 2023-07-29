NAME = webserv
SOURCES = Sources

SRCS_DIRS = acceptConnectionAndGetRequestData init \
			parseConfigFile parseRequest response cgi main

SOURCESDIRS = $(foreach dir, $(SRCS_DIRS), $(addprefix $(SOURCES)/, $(dir)))

VPATH = $(SOURCESDIRS)

SOURCES := $(foreach dir,$(SOURCESDIRS),$(wildcard $(dir)/*.cpp))

OBJS = ${SOURCES:.cpp=.o}

CC = c++
CPPFLAGS = -std=c++98 -Werror -Wextra -Wall
RM = rm -rf

all: $(NAME)

%.o: %.cpp  webserv.hpp
	@$(CC) $(CPPFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(OBJS) -o $(NAME)
	@echo "\033[92m|  Compilation Done  |\033[00m"
# @rm req*

clean:
	@$(RM) ${OBJS}
	@echo "|  CLEANED |"

fclean: clean
	@$(RM) $(NAME)

re: fclean all


