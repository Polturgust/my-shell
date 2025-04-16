##
## EPITECH PROJECT, 2024
## makefils
## File description:
## f
##

SRCS	=	mysh.c \
			src/signal.c \
			src/env.c \
			src/env2.c \
			src/cd.c \
			src/builtin.c \
			src/pipe.c \
			src/redirect.c \
			src/modif_prompt.c \
			src/karentede.c \
			src/alias.c \

NAME	=	mysh

OBJS	=	$(SRCS:.c=.o)

CFLAGS	=	-I./include/ -g3 -L./lib -lmy -llist -Wall -Wextra

all		:	$(NAME)

$(NAME)	:	$(OBJS)
		@make -C lib
		@gcc $(OBJS) -o $(NAME) $(CFLAGS)
		cp $(NAME) tests/

clean	:
		@make clean -C lib
		@rm -rf $(OBJS)

fclean	:	clean
		@make fclean -C lib
		@rm $(NAME)

re		:	fclean all
