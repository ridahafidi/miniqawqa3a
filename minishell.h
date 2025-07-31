/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:23:36 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/31 18:45:05 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define EXIT_BUILTIN_ERROR 2
#define EXIT_EXEC_ERROR 126
#define EXIT_CMD_NOT_FOUND 127
#define EXIT_INVALID_EXIT 128
#define EXIT_SIGINT_CODE 130
#define EXIT_SIGQUIT_CODE 131

#include <signal.h>
#include <unistd.h>

// Global variable for signal handling - only stores signal number
extern int g_signum;
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include "libft/libft.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "parsing.h"

typedef struct s_pid
{
    int right_pid;
    int left_pid;
}   t_pid;

typedef struct s_fd
{
    int in;
    int out;
}   t_fd;

void    execute_command(t_tree *root, t_fd *fd, char **env, char **exported, int *exit_status);
void    free_tree(t_tree **root);
t_tree *handle_redirections(t_tree *root, int *in, int *out, char **env, int *exit_status);
int     is_builtin(char *cmd);
void rdirectin_out(int in, int out);
int handle_builtins(t_tree *root, t_fd *fd, char ***env, char ***exported, int *exit_status);
void    execution(t_tree *root, t_fd *fd, char ***env, char ***exported, int *exit_status);
void    free_array(char **str);
char    **copy_env(char **env);
int ft_strcmp(char *s1, char *s2);
void reset_signals_for_child(void);
int    initialize(t_tree *root, t_fd *fd, char ***env, char ***exported, int *exit_status);
void	sigint_handler(int sig);
void	ctrl_d_handle(int sig, int exit_status);
void	heredoc_sigint_handler(int sig);
void	child_sigint_handler(int sig);
int     check_received_signal(int *exit_status);
int     find_equal(char *str);
int     find_start(char *s);
char	**expand(char **argv, char **env, int exit_status);
int     compare_var_env(char *arg, char **env);
void strip_quotes_from_tokens(char **tokens, int skip_heredoc_delimiter);
int	handle_dot_command(t_tree *root, char **env, char **exported, int i);
int    is_only_spaces(char *str);

// Function prototypes for builtins
int	ft_export(char **argv, char ***env, char ***exported, int *exit_status);
void add_var(char **argv, char ***env, char ***exported, int *exit_status);

#endif