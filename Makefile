SRCS	=	server.cpp \
			client.cpp


OBJS		=	$(SRCS:.cpp=.o)

CC		=	c++

FLAGS	=	-Wall -Wextra -Werror -std=c++98

NAME	=	webserv

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(FLAGS) $(SRCS) -o $(NAME)

%.o : %.cpp
	$(CC) $(FLAGS) -c $<

clean:
	rm -rf *.o

fclean: clean
	rm -rf $(NAME)

re : fclean all