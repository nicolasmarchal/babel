CC = g++

SRC = 			./srcs/main.cpp				\
			./srcs/Server.cpp				\
			./Networking/srcs/SocketManager.cpp	\
			./Networking/srcs/UnixSocket.cpp	\
			./srcs/Client.cpp				\
			./srcs/Command.cpp				\
			./srcs/AFile.cpp				\
			./Networking/srcs/ISocket.cpp				\
			./srcs/UnixFile.cpp

OBJ = $(SRC:.cpp=.o)

CXXFLAGS = -Wall -Wextra -std=c++11 -I../protocol/ -I./Networking/incs -I./include

NAME = server

RM = rm -f

all: $(NAME)

$(NAME): $(OBJ)
					$(CC) -o $(NAME) $(OBJ)

clean:
				$(RM) $(OBJ)

fclean: clean
				$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
