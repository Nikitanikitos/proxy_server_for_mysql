NAME	=	proxy_server
FUNC	=	src/bytes/bytes.cpp \
			src/Client/Client.cpp \
			src/Server/Server.cpp \
			src/Exception/Exception.cpp \
			src/ThreadPool/ThreadPool.cpp \
			src/workers.cpp \
			src/main.cpp

INC		=	-I./src/bytes/ \
			-I./src/Client/ \
			-I./src/ThreadPool/ \
			-I./src/Exception/ \
			-I./src/Server/

CC		=	clang++
FLAGS	=	-Wall -Wextra -Werror -pthread
OBJ		=	$(FUNC:.cpp=.o)

all: $(NAME)
	@echo "\033[32m[+] Make completed\033[0m"

%.o: %.cpp
	@$(CC) $(FLAGS) $(INC) -c $< -o $@

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) $(INC) $(LFT_FLG) -o $(NAME)

clean:
	/bin/rm -f $(OBJ)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
