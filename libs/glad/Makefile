NAME=libglad.a

CC=gcc
FLAGS = -Wall -Werror -Wextra

SRC=glad.c

INC_PATH=include/
INC=include/glad/glad.h include/KHR/khrplatform.h

OBJS=$(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@ar rc $(NAME) $(OBJS)
	@ranlib $(NAME)
	@echo "$(GRE)Glad successfully compiled !$(DEF)"

%.o: %.c $(INC)
	@tput civis
	@printf " Compiling $<"
	@printf "                                       \\r"
	@tput cnorm
	@$(CC) $(FLAGS) -I$(INC_PATH) -c $<

clean: 
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

