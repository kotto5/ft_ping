NAME = ping
CC = gcc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g
DFLAGS = -MMD -MP

HEADER_DIR = includes
HEADERS	=	$(wildcard $(HEADER_DIR)/*.h)
SRCDIR	=	srcs
OBJDIR	=	objs
INCLUDES = -I$(HEADER_DIR) -Ilibft/includes

SRCS	=	$(wildcard $(SRCDIR)/*.c)
OBJS	=	$(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.o)))
DEPS	=	$(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.d)))

LIBFTDIR = ./libft
LIBFT = $(LIBFTDIR)/libft.a

all: $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	-mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(INCLUDES) -o $@

$(LIBFT): FORCE
	make -C ./$(LIBFTDIR)

FORCE:

clean:
	rm -f $(OBJ) $(DEPENDS)
	make clean -C $(LIBFTDIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFTDIR)

NAME_DEBUG = debugfile
CFLAGS_DEBUG = -g

$(NAME_DEBUG): $(LIBFT) $(LIBMLX) $(OBJ)
	@printf "$(GREEN)"
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(INCLUDES) -L/usr/X11R6/lib -lmlx -lX11 -lXext -framework OpenGL -framework AppKi -o $@
	@printf "$(RESET)"

# for online
# $(NAME_DEBUG): $(LIBFT) $(LIBMLX) $(OBJ)
# 	@printf "$(GREEN)"
# 	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LIBMLX) $(INCLUDES) $(LIBMLXFLAGS) -lm -o $@
# 	@printf "$(RESET)"

debug: CFLAGS += $(CFLAGS_DEBUG)
debug: $(NAME_DEBUG)

re: fclean all

.PHONY: all clean fclean re FORCE

-include $(DEPENDS)
