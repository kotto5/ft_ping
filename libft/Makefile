NAME = libft.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror

M_SRCS = $(wildcard *.c)

INCLUDES = -I./includes

M_OBJS = $(M_SRCS:%.c=%.o)
B_OBJS = $(B_SRCS:%.c=%.o)

OBJS = $(M_OBJS)
ifdef WITH_BONUS
OBJS += $(B_OBJS)
endif

%.o:%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

all:$(NAME)

$(NAME):$(OBJS)
	ar r $(NAME) $(OBJS)

.PHONY:
	clean;

bonus:
	make WITH_BONUS=1

clean:
	rm -f $(M_OBJS) $(B_OBJS)

fclean:clean
	rm -f $(NAME)

re:fclean all
