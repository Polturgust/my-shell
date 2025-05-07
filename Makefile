##
## EPITECH PROJECT, 2024
## makefils
## File description:
## f
##

MAIN = main.c \

SRCS	=	mysh.c \
			execcommand.c \
			src/signal.c \
			src/env.c \
			src/env2.c \
			src/cd.c \
			src/builtin.c \
			src/pipe.c \
			src/redirect.c \
			src/modif_prompt.c \
			src/exit.c \
			src/alias.c \
			src/history.c \
			src/history2.c \
			src/variables/builtin_set_unset.c \
			src/variables/special_variables.c \
			src/variables/variables.c \
			src/variables/handle_var_builtins.c \
			src/globbings.c \
			src/and_or.c \
			src/skip.c \
			src/parenthesis.c \


NAME	=	42sh

OBJS	=	$(SRCS:.c=.o) $(MAIN:.c=.o)

LIB =  -L./lib -lmy -llist

INCLUDE = -I./include/

CFLAGS	= $(INCLUDE) -g3 $(LIB) -Wall -Wextra

all		: buildlib	$(OBJS)
		@gcc $(OBJS) -o $(NAME) $(CFLAGS)
		@cp $(NAME) tests/

clean	:
		@rm -fr $(OBJS)
		@rm -fr *.gcno
		@rm -fr *.gcda

fclean	:	clean fcleanlib
		@rm -fr $(NAME) $(TNAME)
		@rm -rf tests/$(NAME)

buildlib:
		@make -C ./lib/

fcleanlib:
		@make fclean -C ./lib/

re		:	fclean all

TSRC = tests/tests.c \

TNAME = unit_tests

TLIB = $(LIB) -lcriterion --coverage

unit_tests: clean buildlib
		@gcc $(TSRC) $(SRCS) -o $(TNAME) $(INCLUDE) $(TLIB)

tests_run: unit_tests
		@./unit_tests && gcovr
