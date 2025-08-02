/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:23:36 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 20:30:17 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include "libft/libft.h"
# include "parsing.h"

# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
# define EXIT_BUILTIN_ERROR 2
# define EXIT_EXEC_ERROR 126
# define EXIT_CMD_NOT_FOUND 127
# define EXIT_INVALID_EXIT 128
# define EXIT_SIGINT_CODE 130
# define EXIT_SIGQUIT_CODE 131

// Global variable for signal handling - only stores signal number
extern int	g_signum;

typedef struct s_pid
{
	int		right_pid;
	int		left_pid;
}			t_pid;

typedef struct s_fd
{
	int		in;
	int		out;
}			t_fd;

typedef struct s_data
{
	char	***env;
	char	***exported;
	t_fd	*fds;
	t_pid	*pid;
}			t_data;

typedef struct s_expand_context
{
	char	**env;
	int		status;
	int		*doll;
	int		*in_single_quote;
	int		*in_double_quote;
	int		heredoc;
	char	*result;
	int		*i;
}			t_expand_context;

typedef struct s_dollar_context
{
	char	**env;
	int		status;
	int		*doll;
}			t_dollar_context;

void		check_path(char *path);
char		*ret_braces(char *result, char *var_value);
char		*ret_handle_dollar(char *result, int *i);
int			handle_heredoc(char *delimiter, char **env);
t_tree		*check_ambiguous_redirection(t_tree *root, t_tree *cmd,
				int *exit_status);
t_tree		*handle_append_heredoc(t_tree *root, t_data *data,
				int *exit_status);
void		handle_execve_error(t_tree *root, char *path, char **env,
				char **exported);
void		update_shlvl(char ***env);
int			get_exit_status(int exit_status);
void		redirecting(int in, int out);
char		*handle_relative_path(t_tree *root, char **env, char **exported,
				int i);
void		handle_path_errors(t_tree *root, char **env, char **exported,
				int i);
char		*handle_absolute_path(t_tree *root, char **env, char **exported,
				int i);
int			is_directory(const char *path);
void		handle_redirection_execution(t_tree *cmd, t_data *data,
				int *exit_status);
void		handle_redirection_error(t_fd *fd);
void		handle_pipe(t_data *data, t_tree *root, int *exit_status);
int			process_input_line(char *input, char ***env, char ***exported,
				int *exit_status);
int			handle_tokenization_error(t_fd *fds, t_data *data,
				int *exit_status);
int			handle_parsing_error(char **tokens, t_data *data, int *exit_status);
char		**handle_env_i(void);
void		execute_command(t_tree *root, t_data *data);
void		free_tree(t_tree **root);
t_tree		*handle_redirections(t_tree *root, t_data *data, int *exit_status);
int			is_builtin(char *cmd);
void		rdirectin_out(int in, int out);
int			handle_builtins(t_tree *root, t_data *data, int *exit_status);
void		execution(t_tree *root, t_data *data, int *exit_status);
void		free_array(char **str);
char		**copy_env(char **env);
int			ft_strcmp(char *s1, char *s2);
void		reset_signals_for_child(void);
int			initialize(t_tree *root, t_data *data, int *exit_status);
void		sigint_handler(int sig);
void		ctrl_d_handle(int sig, int exit_status);
void		heredoc_sigint_handler(int sig);
void		child_sigint_handler(int sig);
int			check_received_signal(int *exit_status);
int			find_equal(char *str);
int			find_start(char *s);
char		**expand(char **argv, char **env, int exit_status);
int			compare_var_env(char *arg, char **env);
void		strip_quotes_from_tokens(char **tokens, int skip_heredoc_delimiter);
int			handle_dot_command(t_tree *root, char **env, char **exported,
				int i);
int			is_only_spaces(char *str);
int			ft_export(char **argv, char ***env, char ***exported,
				int *exit_status);
void		add_var(char **argv, char ***env, char ***exported,
				int *exit_status);
void		print_format(char **export);
void		sort_export(char **export);
void		add_or_update_exported(char *var, char ***exported);
int			is_valid_identifier(char *str);
void		handle_regular_assignment(char *var, char ***env, int len);
void		handle_append_env(char *var, char ***env, int len,
				int var_name_len);
int			check_append_operation(char *var);
int			handle_append_exported(char *var, char ***exported, int len);
int			is_in_exported(char *var, char **exported);
void		ft_swap(char **export, int i, int j);
char		*build_append_var(char *var, int var_name_len);
void		free_mem(t_tree *root, t_fd *fd, t_data *data);
int			ret_ex_code(char **argv);
int			many_args(void);
void		exit_error(char **argv);
void		clean_exit(t_tree *root, t_data *data, int exit_status);
void		ft_swap_ptr(char ***arr, int i, int j);
int			validate_and_get_path(char **argv, char **path);
void		update_env(char ***env, char *new_cwd, char *cwd);
int			handle_directory_change(char *path, char ***env);
char		*ft_getenv(char **env, const char *name);
int			count_arguments(char **argv);
void		print_cd_error(char *path, int error_type);
int			check_dir_x(struct stat *file_stat, char *path);
void		forker(t_tree *root, t_data *data, int *exit_status);
int			ft_exit(t_tree *root, t_data *data, int *exit_status);
int			ft_env(char **argv, char **env);
char		*get_path(char *cmd, char **env);
int			ft_unset(char **argv, char ***env, char ***exported);
int			ft_pwd(void);
int			ft_echo(char **argv);
int			ft_cd(char **argv, char ***env);
char		*get_before_dollar(char *str, int dollar_index);
void		process_dollar_sequence(char *str, char *result, int *i, int *ri);
char		*expand_dollars(char *str);
int			check_for_dollar(char *str);
char		*get_braced_var_name(char *str, int *i, int *len);
char		*get_regular_var_name(char *str, int *i, int *len);
char		*get_var_name(char *str);
char		*get_var_value(char *var_name, char **env, int status);
int			find_var_end(char *str);
char		*handle_single_quote(char *result, char *str, int *i, int heredoc);
char		*handle_double_quote(char *result, char *str, int *i, int heredoc);
char		*handle_double_dollar(char *result, int *i);
char		*process_single_quote_section(char *result, char *quoted_section);
char		*process_double_quote_section(char *result, char *quoted_section,
				char **env, int status);
char		*handle_dollar_quote(char *result, char *str, int *i,
				t_dollar_context *ctx);
char		*handle_exit_status(char *result, int *i, int status);
char		*handle_braced_var(char *result, char *str, int *i,
				t_dollar_context *ctx);
char		*handle_regular_var(char *result, char *str, int *i,
				t_dollar_context *ctx);
char		*handle_literal_dollar(char *result, int *i);
char		*handle_regular_char(char *result, char *str, int *i);
char		*handle_dollar_cases(char *result, char *str, int *i,
				t_dollar_context *ctx);
char		*process_character(char *result, char *str, int *i,
				t_expand_context *ctx);
#endif