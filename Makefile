# Compiler settings

CC		=	c++

FLAGS	=	-Wall -Wextra -Werror -std=c++98 -Iincludes

NAME	=	webserv

# directories

SRCSDIR		:=	srcs
OBJSDIR		:=	objs
CONFIGDIR	:=	Config
SERVERDIR	:=	Server

# sources


CPPCONFIG	:=	Config.cpp

CPPSERVER	:=	Server.cpp

CFILES		:=	main_test_parsing_config_file.cpp \
				$(foreach F,$(CPPCONFIG),$(CONFIGDIR)/$(F)) \
				$(foreach F,$(CPPSERVER),$(SERVERDIR)/$(F)) \

SRCS		:= $(foreach F,$(CFILES),$(SRCSDIR)/$(F))

# objects
OBJS		:= $(patsubst $(SRCSDIR)/%.c,$(OBJSDIR)/%.o,$(SRCS))

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

%.o : %.cpp
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf *.o

fclean: clean
	rm -rf $(NAME)

re : fclean all