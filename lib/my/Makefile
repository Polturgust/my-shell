##
## EPITECH PROJECT, 2024
## makefile
## File description:
## makefile
##

NAME	=	libmy.a

SRCS	=	my_putstr.c	\
		my_malloc.c \
		my_str_isnum.c	\
		my_revstr.c	\
		my_str_isprintable.c	\
		my_strdup.c \
		my_showmem.c	\
		my_str_isupper.c	\
		my_compute_power_rec.c	\
		my_strlen.c	\
		my_compute_square_root.c	\
		my_sort_int_array.c  my_strlowcase.c	\
		my_find_prime_sup.c	\
		my_strcapitalize.c	\
		my_strncat.c	\
		my_getnbr.c	\
		my_strcat.c	\
		my_strncmp.c	\
		my_isneg.c	\
		my_strcmp.c	\
		my_strncpy.c	\
		my_is_prime.c	\
		my_strcpy.c	\
		my_strstr.c	\
		my_putchar.c	\
		my_str_isalpha.c	\
		my_strupcase.c	\
		my_put_nbr.c	\
		my_str_islower.c	\
		my_swap.c	\
		str_to.c \
		my_strconcat.c \
		free_tab.c \

OBJS	=	$(SRCS:.c=.o)

CFLAGS	=  -I../../include/

all	:	$(NAME)

$(NAME)	:	$(OBJS)
		@ar rc $(NAME) $(OBJS)
		@cp $(NAME) ../
		@cp my.h ../../include/my.h

clean	:
		@rm -rf $(OBJS)

fclean	:	clean
		@rm -rf $(NAME)
		@rm ../$(NAME)

re	:	fclean all
