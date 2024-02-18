##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## make
##

NAME = plazza

SRC = ./src/main.cpp ./src/Reception.cpp ./src/Kitchen.cpp ./src/Pizza.cpp \
		./src/encapsulations/Process.cpp ./src/encapsulations/SharedMemory.cpp \
		./src/encapsulations/MessageQueue.cpp
OBJ = $(SRC:.cpp=.o)

CXXFLAGS += -I ./
CXXFLAGS += -I ./include/

CXXFLAGS += -pthread -Wall -Wextra

all: $(NAME)

$(NAME): $(OBJ)
	g++ -o $(NAME) $(OBJ) -pthread

clean:
	rm -fr $(OBJ)

fclean: clean
	rm -fr $(NAME)

re: fclean all

.PHONY: all clean fclean re
