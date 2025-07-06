/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:39:56 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/06 21:29:53 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_heredoc(char *delimiter, char **env)
{
    int pipefd[2];
    char    *line[2];
    char    *tmp_delimiter[2];
    char    **expanded;
    int     i;

    if (pipe(pipefd) == -1)
    {
        perror("pipe failed");
        return -1;
    }

    signal(SIGINT, heredoc_sigint_handler); // Handle SIGINT during heredoc
    tmp_delimiter[0] = ft_strdup(delimiter);
    tmp_delimiter[1] = NULL;
    strip_quotes_from_tokens(tmp_delimiter, 0);
    while (1)
    {
        line[0] = readline("> ");
        line[1] = NULL;
        if (!line[0]) // Handle EOF (Ctrl+D)
        {
            write(STDOUT_FILENO, "\n", 1);
            break;
        }
        if (!ft_strcmp(line[0], tmp_delimiter[0])) // Check if delimiter is matched
            break;
        if ((delimiter[0] == '\'' && delimiter[ft_strlen(delimiter) - 1] == '\'')  || (delimiter[0] == '\"' && delimiter[ft_strlen(delimiter) - 1] == '\"'))
        {
            write(pipefd[1], line[0], ft_strlen(line[0])); // Write to pipe
            write(pipefd[1], "\n", 1);
        }
        else
        {
            expanded = expand(line, env, exit_status);
            write(pipefd[1], expanded[0], ft_strlen(expanded[0])); // Write to pipe
            write(pipefd[1], "\n", 1);
            free_array(expanded);
        }
    }
    close(pipefd[1]); // Close write end of the pipe
    signal(SIGINT, sigint_handler); // Restore default SIGINT handler
    return pipefd[0]; // Return read end of the pipe
}

void    append(t_tree *root, int *in, int *out,int flag)
{
    if (root->type == APPEND && root->file_name)
    {
        if (flag)
        {
            int fd = open(root->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(root->file_name, 2);
                ft_putstr_fd(": Permission denied\n", 2);
                // exit(EXIT_FAILURE);
                exit_status = 1;
            }
            return ;
        }
        else
        {
            *out = open(root->file_name, O_WRONLY | O_CREAT | O_APPEND , 0644);
            if (*out == -1)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(root->file_name, 2);
                ft_putstr_fd(": Permission denied\n", 2);
                // exit(EXIT_FAILURE);
                exit_status = 1;
            } 
        }
    }
}

void    less_and_greater(t_tree *root, int *in , int *out, int flag)
{
    if (root->type == LESS && root->file_name)
    {
        if (flag)
        {
            int fd = open(root->file_name, O_RDONLY);
            if (fd == -1)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(root->file_name, 2);
                ft_putstr_fd(": No such file or directory\n", 2);
                // exit(EXIT_FAILURE);
                exit_status = 1;
                return ;
            }
            return ;
        }
        else
        {
            *in = open(root->file_name, O_RDONLY);
            if (*in == -1)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(root->file_name, 2);
                ft_putstr_fd(": No such file or directory\n", 2);
                // exit(EXIT_FAILURE);
                exit_status = 1;
                return ;
            }
        }
    }
    else if (root->type == GREATER && root->file_name)
    {
        if (flag)
        {
            int fd = open(root->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(root->file_name, 2);
                ft_putstr_fd(": Permission denied\n", 2);
                // exit(EXIT_FAILURE);
                exit_status = 1;
                return ;
            }
        }
        else
        {
            *out = open(root->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (*out == -1)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(root->file_name, 2);
                ft_putstr_fd(": Permission denied\n", 2);
                // exit(EXIT_FAILURE);
                exit_status = 1;
                return ;
            }
        }
    }
}

void handle_redirections(t_tree *root, int *in, int *out, int flag, char **env)
{
    if (!root)
        return;
    if ((root->type == LESS || root->type == GREATER) && root->file_name)
        less_and_greater(root, in, out, flag);
    else if (root->type == APPEND && root->file_name)
        append(root, in, out, flag);
    else if (root->type == HEREDOC && root->file_name)
    {
        if (flag)
            return;
        *in = handle_heredoc(root->file_name, env); // Use heredoc's read end
        if (*in == -1)
        {
            perror("heredoc failed");
            exit(1);
        }
    }
}