/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:23:36 by rhafidi           #+#    #+#             */
/*   Updated: 2025/05/20 17:24:50 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include "libft/libft.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "parsing.h"
// typedef enum type
// {
//     COMMAND,
//     LESS,
//     GREATER,
//     APPEND,
//     PIPE
// }           type;

// typedef struct s_tree
// {
//     struct s_tree *left;
//     struct s_tree *right;
//     char *file_name;
//     char **command;
//     type type;
// }              t_tree;

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

void    execute_command(t_tree *root, int in, int out, char **env);
void    free_tree(t_tree **root);
void    handle_redirections(t_tree *root, int *in, int *out);
int     is_builtin(char *cmd);
void rdirectin_out(int in, int out);
int handle_builtins(t_tree *root, int in, int out, char ***env, int status);
void    execution(t_tree *root,t_fd *fd, char ***env);
void    free_array(char **str);
char    **copy_env(char **env);
int ft_strcmp(char *s1, char *s2);
void reset_signals_for_child(void);
int    initialize(t_tree *root, t_fd *fd, char ***env);
void	sigint_handler(int sig);
void	ctrl_d_handle(int sig);
void	heredoc_sigint_handler(int sig);
void	child_sigint_handler(int sig);
int find_equal(char *str);
extern int exit_status;

#endif