##
## EPITECH PROJECT, 2024
## 42sh
## File description:
## Makefile
##

NAME	=	42sh

SRCS	=	$(shell find src -name '*.c' | sort)

OBJS	=	$(SRCS:.c=.o)

CPPFLAGS	+=	-I./include/ -I./src/editor -I./src/expand

CFLAGS	+=	-Wall -Wextra

all:	$(NAME)

$(NAME):	$(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)

clean:
	rm -f $(OBJS)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

tests_run:	$(NAME)
	python3 tests/interactive.py ./$(NAME)

.PHONY: all clean fclean re tests_run
