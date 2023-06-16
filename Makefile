# Compiler settings

CC		=	c++

FLAGS	=	-Wall -Wextra -Werror -std=c++98 -Iincludes

NAME	=	webserv

# directories

SRCSDIR		:=	srcs
OBJSDIR		:=	objs
CONFIGDIR	:=	Config
SERVERDIR	:=	Server
UTILSDIR	:=	Utils
REQUESTDIR	:=	Request
CLIENTDIR	:=	Client
RESPONSEDIR :=	Response

# sources


CPPCONFIG	:=	Config.cpp \
				Network.cpp

CPPSERVER	:=	Server.cpp

CPPCLIENT	:=	Client.cpp

CPPUTILS	:=	utils.cpp

CPPREQUEST	:=	request.cpp \
				geters.cpp  \
				parsing_rquest.cpp \
				util_request.cpp \
				Post.cpp \
				Get.cpp

CPPRESPONSE  := Response_msg.cpp \
				Response.cpp \
				Response_Errors.cpp \
				Utils_respnonse.cpp 

CFILES		:=	main_test_request_config.cpp \
				$(foreach F,$(CPPCONFIG),$(CONFIGDIR)/$(F)) \
				$(foreach F,$(CPPSERVER),$(SERVERDIR)/$(F)) \
				$(foreach F,$(CPPUTILS),$(UTILSDIR)/$(F)) \
				$(foreach F,$(CPPCLIENT),$(CLIENTDIR)/$(F)) \
				$(foreach F,$(CPPREQUEST),$(REQUESTDIR)/$(F)) \
				$(foreach F,$(CPPRESPONSE),$(RESPONSEDIR)/$(F))

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