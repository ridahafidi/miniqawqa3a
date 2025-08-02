# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/02 18:00:00 by rhafidi           #+#    #+#              #
#    Updated: 2025/08/02 18:00:00 by rhafidi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ================================ VARIABLES ================================ #

NAME		= minishell

# Compiler and flags
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g3
INCLUDES	= -I. -I./libft -I./exec_norm
LIBS		= -L./libft -lft -lreadline
RM			= rm -f

# Colors for output
RED			= \033[0;31m
GREEN		= \033[0;32m
YELLOW		= \033[0;33m
BLUE		= \033[0;34m
MAGENTA		= \033[0;35m
CYAN		= \033[0;36m
WHITE		= \033[0;37m
RESET		= \033[0m

# Directories
LIBFT_DIR	= ./libft
EXEC_DIR	= ./exec_norm

# Source files in main directory
SRCS		= absolute_path.c \
			  add_delimiter.c \
			  append_heredoc.c \
			  build_tree.c \
			  builtins.c \
			  cd_handler.c \
			  cd_main.c \
			  cd_utils.c \
			  command_exec.c \
			  dot_command_handler.c \
			  echo.c \
			  env_unset.c \
			  exec_err.c \
			  exec_redirection.c \
			  exec_utils.c \
			  exit.c \
			  exit_utils.c \
			  expand_dollar.c \
			  expand_dollar_utils.c \
			  expand_main.c \
			  expand_quotes.c \
			  expand_utils.c \
			  expand_vars.c \
			  export_env.c \
			  export_env_ops.c \
			  export_format.c \
			  export_helper.c \
			  export_main.c \
			  export_utils.c \
			  forker.c \
			  forker_utils.c \
			  free_tree.c \
			  hanlde_env_i.c \
			  helper_functions.c \
			  heredoc_handler.c \
			  initialize_exec.c \
			  insert_spaces.c \
			  main.c \
			  parse_tokens.c \
			  parse_tokens1.c \
			  parse_tokens2.c \
			  parse_tokens3.c \
			  parse_tokens4.c \
			  parsing_err.c \
			  path_utills.c \
			  pipe.c \
			  process_input.c \
			  pwd.c \
			  redirections.c \
			  redirections_parsing.c \
			  redirections_parsing1.c \
			  relative_path.c \
			  signals.c \
			  syntax_error.c \
			  syntax_error_utils.c \
			  tokenize.c \
			  tokenize1.c \
			  tokenize2.c \
			  tokenize3.c \
			  update_shlvl.c \
			  utils.c

# Source files in exec_norm directory
EXEC_SRCS	= expand.c \
			  expand_utils.c \
			  expand_vars.c \
			  expand_quotes.c \
			  expand_dollar.c \
			  expand_main.c

# Add directory prefix to exec sources
EXEC_SRCS_FULL = $(addprefix $(EXEC_DIR)/, $(EXEC_SRCS))

# All source files
ALL_SRCS	= $(SRCS) $(EXEC_SRCS_FULL)

# Object files
OBJS		= $(SRCS:.c=.o)
EXEC_OBJS	= $(EXEC_SRCS_FULL:.c=.o)
ALL_OBJS	= $(OBJS) $(EXEC_OBJS)

# Header files for dependency tracking
HEADERS		= minishell.h parsing.h $(EXEC_DIR)/expand.h

# ================================== RULES ================================== #

# Default target
all: libft $(NAME)

# Build the main executable
$(NAME): $(ALL_OBJS) $(LIBFT_DIR)/libft.a
	@echo "$(CYAN)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(ALL_OBJS) $(LIBS) -o $(NAME)
	@echo "$(GREEN)✅ $(NAME) compiled successfully!$(RESET)"

# Compile object files from main directory
%.o: %.c $(HEADERS)
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile object files from exec_norm directory  
$(EXEC_DIR)/%.o: $(EXEC_DIR)/%.c $(HEADERS)
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Build libft
libft:
	@echo "$(BLUE)Building libft...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory
	@echo "$(GREEN)✅ Libft compiled successfully!$(RESET)"

# Clean object files
clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@$(RM) $(ALL_OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean --no-print-directory
	@echo "$(GREEN)✅ Object files cleaned!$(RESET)"

# Clean everything
fclean: clean
	@echo "$(RED)Cleaning executable and libraries...$(RESET)"
	@$(RM) $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean --no-print-directory
	@echo "$(GREEN)✅ Everything cleaned!$(RESET)"

# Rebuild everything
re: fclean all

# ================================ NORMINETTE ================================ #

# Run norminette on all .c and .h files in current directory
norm:
	@echo "$(MAGENTA)🔍 Running norminette on main directory...$(RESET)"
	@norminette *.c *.h || true
	@echo "$(MAGENTA)🔍 Running norminette on libft...$(RESET)"
	@norminette $(LIBFT_DIR)/*.c $(LIBFT_DIR)/*.h || true
	@echo "$(MAGENTA)🔍 Running norminette on exec_norm...$(RESET)"
	@norminette $(EXEC_DIR)/*.c $(EXEC_DIR)/*.h || true
	@echo "$(GREEN)✅ Norminette check completed!$(RESET)"

# Run norminette only on main directory files
norm-main:
	@echo "$(MAGENTA)🔍 Running norminette on main directory only...$(RESET)"
	@norminette *.c *.h || true
	@echo "$(GREEN)✅ Main directory norminette check completed!$(RESET)"

# Run norminette only on libft
norm-libft:
	@echo "$(MAGENTA)🔍 Running norminette on libft only...$(RESET)"
	@norminette $(LIBFT_DIR)/*.c $(LIBFT_DIR)/*.h || true
	@echo "$(GREEN)✅ Libft norminette check completed!$(RESET)"

# Run norminette only on exec_norm
norm-exec:
	@echo "$(MAGENTA)🔍 Running norminette on exec_norm only...$(RESET)"
	@norminette $(EXEC_DIR)/*.c $(EXEC_DIR)/*.h || true
	@echo "$(GREEN)✅ Exec_norm norminette check completed!$(RESET)"

# ============================= FORMAT TARGETS ============================== #

# Format all files in main directory
format: format-main
	@echo "$(GREEN)🎉 All files formatted successfully!$(RESET)"

format-main:
	@echo "$(CYAN)🔧 Formatting main directory files...$(RESET)"
	@if [ -f "$(HOME)/.local/bin/c_formatter_42" ]; then \
		for file in *.c *.h; do \
			if [ -f "$$file" ]; then \
				echo "  📄 Formatting $$file..."; \
				$(HOME)/.local/bin/c_formatter_42 < "$$file" > "$$file.tmp" && mv "$$file.tmp" "$$file" || rm -f "$$file.tmp"; \
			fi; \
		done; \
		echo "$(GREEN)✅ Main directory formatting completed!$(RESET)"; \
	else \
		echo "$(RED)❌ Error: c_formatter_42 not found. Install with: pip3 install --user c_formatter_42$(RESET)"; \
		exit 1; \
	fi

format-exec:
	@echo "$(CYAN)🔧 Formatting exec_norm directory files...$(RESET)"
	@if [ -f "$(HOME)/.local/bin/c_formatter_42" ]; then \
		cd $(EXEC_DIR) && for file in *.c *.h; do \
			if [ -f "$$file" ]; then \
				echo "  📄 Formatting $$file..."; \
				$(HOME)/.local/bin/c_formatter_42 < "$$file" > "$$file.tmp" && mv "$$file.tmp" "$$file" || rm -f "$$file.tmp"; \
			fi; \
		done; \
		echo "$(GREEN)✅ Exec_norm directory formatting completed!$(RESET)"; \
	else \
		echo "$(RED)❌ Error: c_formatter_42 not found. Install with: pip3 install --user c_formatter_42$(RESET)"; \
		exit 1; \
	fi

format-all: format-main format-exec
	@echo "$(GREEN)🎉 All directories formatted successfully!$(RESET)"

# ================================= TESTING ================================== #

# Run the program in test mode
test: $(NAME)
	@echo "$(CYAN)🧪 Starting minishell for testing...$(RESET)"
	@./$(NAME)

# Run with valgrind for memory leak detection
valgrind: $(NAME)
	@echo "$(CYAN)🔍 Running with valgrind...$(RESET)"
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=file.sup ./$(NAME)

# Run with specific test commands
test-commands: $(NAME)
	@echo "$(CYAN)🧪 Testing basic commands...$(RESET)"
	@echo "echo 'Hello World'" | ./$(NAME)
	@echo "pwd" | ./$(NAME)
	@echo "ls -la" | ./$(NAME)

# ================================== DEBUG ================================== #

# Compile with debug flags
debug: CFLAGS += -g3 -fsanitize=address
debug: $(NAME)
	@echo "$(GREEN)✅ Debug version compiled!$(RESET)"

# Show compilation info
info:
	@echo "$(BLUE)📋 Compilation Information:$(RESET)"
	@echo "$(YELLOW)Compiler:$(RESET) $(CC)"
	@echo "$(YELLOW)Flags:$(RESET) $(CFLAGS)"
	@echo "$(YELLOW)Includes:$(RESET) $(INCLUDES)"
	@echo "$(YELLOW)Libraries:$(RESET) $(LIBS)"
	@echo "$(YELLOW)Source files:$(RESET) $(words $(ALL_SRCS)) files"
	@echo "$(YELLOW)Object files:$(RESET) $(words $(ALL_OBJS)) files"

# Show help
help:
	@echo "$(BLUE)📚 Available targets:$(RESET)"
	@echo "$(GREEN)  all$(RESET)          - Build the project (default)"
	@echo "$(GREEN)  clean$(RESET)        - Remove object files"
	@echo "$(GREEN)  fclean$(RESET)       - Remove all generated files"
	@echo "$(GREEN)  re$(RESET)           - Rebuild everything"
	@echo "$(GREEN)  libft$(RESET)        - Build only libft"
	@echo ""
	@echo "$(MAGENTA)  norm$(RESET)         - Run norminette on all files"
	@echo "$(MAGENTA)  norm-main$(RESET)    - Run norminette on main directory only"
	@echo "$(MAGENTA)  norm-libft$(RESET)   - Run norminette on libft only"
	@echo "$(MAGENTA)  norm-exec$(RESET)    - Run norminette on exec_norm only"
	@echo ""
	@echo "$(BLUE)  format$(RESET)       - Format main directory files with c_formatter_42"
	@echo "$(BLUE)  format-main$(RESET)  - Format main directory files"
	@echo "$(BLUE)  format-exec$(RESET)  - Format exec_norm directory files"
	@echo "$(BLUE)  format-all$(RESET)   - Format all directories"
	@echo ""
	@echo "$(CYAN)  test$(RESET)         - Run the program"
	@echo "$(CYAN)  valgrind$(RESET)     - Run with valgrind"
	@echo "$(CYAN)  test-commands$(RESET) - Test basic commands"
	@echo "$(CYAN)  debug$(RESET)        - Build with debug flags"
	@echo ""
	@echo "$(YELLOW)  info$(RESET)         - Show compilation information"
	@echo "$(YELLOW)  help$(RESET)         - Show this help message"

# ================================= PHONIES ================================== #

.PHONY: all clean fclean re libft norm norm-main norm-libft norm-exec format format-main format-exec format-all test valgrind test-commands debug info help

# ============================= END OF MAKEFILE ============================ #
