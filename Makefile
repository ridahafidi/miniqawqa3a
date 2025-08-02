
NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
INCLUDES	= -I. -I./libft
LIBS		= -L./libft -lft -lreadline
RM			= rm -f

LIBFT_DIR	= ./libft


SRCS		= absolute_path.c add_delimiter.c append_heredoc.c build_tree.c \
			  builtins.c cd_handler.c cd_handler_utils.c cd_main.c cd_utils.c \
			  command_exec.c dot_command_handler.c echo.c env_unset.c exec_err.c \
			  exec_redirection.c exec_utils.c exit.c exit_utils.c expand_dollar.c \
			  expand_dollar_utils.c expand_main.c expand_quotes.c expand_utils.c \
			  expand_vars.c export_env.c export_env_ops.c export_format.c \
			  export_helper.c export_main.c export_utils.c forker.c forker_utils.c \
			  free_tree.c hanlde_env_i.c helper_functions.c heredoc_handler.c \
			  initialize_exec.c insert_spaces.c main.c parse_tokens.c parse_tokens1.c \
			  parse_tokens2.c parse_tokens3.c parse_tokens4.c parsing_err.c \
			  path_utills.c pipe.c process_input.c pwd.c redirections.c \
			  redirections_parsing.c redirections_parsing1.c relative_path.c \
			  signals.c syntax_error.c syntax_error_utils.c tokenize.c tokenize1.c \
			  tokenize2.c tokenize3.c update_shlvl.c utils.c


OBJS		= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): libft $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(NAME)

libft:
	make -C $(LIBFT_DIR) -f MAKEFILE

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS)
	make -C $(LIBFT_DIR) -f MAKEFILE clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT_DIR) -f MAKEFILE fclean

re: fclean all

.PHONY: all clean fclean re libft
