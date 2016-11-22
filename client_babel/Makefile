RM	= rm -f

NAME = client_babel

OS	= $(shell uname)

ifeq ($(OS), Darwin)
LIBDIRECTORY = OSX
else
LIBDIRECTORY = LINUX
endif

SRCS	= 			srcs/GUI/AGUI.cpp \
				srcs/client_babel.cpp \
				srcs/Client.cpp \
				srcs/Networking/ClientUDP.cpp \
				srcs/Networking/Select.cpp \
				srcs/Threading/TaskThread.cpp \
				srcs/Threading/ThreadPool.cpp \
				srcs/GUI/TermGUI.cpp \
				../server/Networking/srcs/SocketManager.cpp \
				../server/Networking/srcs/UnixSocket.cpp \
				../server/Networking/srcs/ISocket.cpp \
				./srcs/Errors/NetworkError.cpp \
				srcs/Audio/AudioEncapsuler.cpp \
				srcs/Audio/EncodeEncapsuler.cpp

SRCS_HPP =			 incs/Threading/AThreadController.hpp


INCS =  			-I./incs \
				-I./incs/GUI \
				-I./incs/Threading \
				-I./incs/Errors \
				-I./incs/Networking \
				-I../server/Networking/incs \
				-I./include \
				-I./incs/Audio \
				-I../protocol

LIB = -L./lib/$(LIBDIRECTORY) -lportaudio -L./lib/$(LIBDIRECTORY) -lopus

CXXFLAGS += -g3 -W -Wall -Wextra -lpthread -std=c++11 $(INCS)

OBJS		=	$(SRCS:.cpp=.o)

all:		$(NAME)

$(NAME): $(OBJS)
		g++ -o $(NAME) $(OBJS) $(LIB) $(CXXFLAGS)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re:	fclean all
