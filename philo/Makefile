NAME = philo

SRC = $(wildcard *.c)

OBJ =  $(SRC:.c=.o)

all : $(NAME)

$(OBJ) : $(SRC)
	@gcc -Wall -Wextra -Werror -c $(SRC)

$(NAME): $(OBJ)
	@gcc -Wall -Wextra -Werror $(OBJ) -o $(NAME)
	@echo "👍👍👍"

clean :
 
	@rm -f $(OBJ) $(OBJ_BONUS)
	@echo "🚮🚮🚮"

fclean : clean
	@rm -f $(NAME) $(NAME_BONUS)
	@echo "🗑️ 🗑️ 🗑️"

re : fclean all