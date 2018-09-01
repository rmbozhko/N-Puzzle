NAME = n_puzzle
CC = g++

SRC = NPuzzle.cpp ProcessingData.cpp Solver.cpp State.cpp SolverHeuristics.cpp SolverAlgos.cpp
OBJ = $(SRC:.cpp=.o)
CFLAGS = -c -O0 -std=c++11 #-Wall -Wextra -Werror
HEADERS = NPuzzle.hpp State.hpp Solver.hpp

all: $(NAME)

$(NAME) : $(OBJ) $(HEADERS)
	@$(CC) $(OBJ) -o $(NAME)
	@echo "\033[0;36m$(NAME) is compiled\033[0m"

%.o : %.cpp $(HEADERS)
	@$(CC) $(CFLAGS) $<

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all