OBJS		=	$(SRCS:.cpp=.o)

CC		=	c++

FLAGS	=	-Wall -Wextra -Werror -std=c++98

NAME	=	webserv

# directories

SRCSDIR		:= srcs
OBJSDIR		:= objs
CONFIGDIR	:=	Config

# sources


CPPCONFIG	:=	Config.cpp

CFILES		:=	main.cpp \
				$(foreach F,$(CPPCONFIG),$(CONFIGDIR)/$(F)) \

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