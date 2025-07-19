/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:39:56 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/19 14:26:36 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_heredoc(char *delimiter, char **env)
{
    int pipefd[2];
    char    *line[2];
    char    *tmp_delimiter[2];
    char    *tmp;
    char    **expanded;
    int     i;

    if (pipe(pipefd) == -1)
    {
        perror("pipe failed");
        return -1;
    }

    signal(SIGINT, heredoc_sigint_handler); // Handle SIGINT during heredoc
    // tmp_delimiter[0] = ft_strdup(delimiter);
    tmp_delimiter[1] = NULL;
    // strip_quotes_from_tokens(tmp_delimiter, 0);
    // tmp = remove_quotes_from_string(delimiter, 1);
    // free(tmp_delimiter)
    tmp_delimiter[0] = remove_quotes_from_string(delimiter, 1);
    // exit (1);
    // printf("%s\n", delimiter);
    while (1)
    {
        line[0] = readline("> ");
        line[1] = NULL;
        if (!line[0]) // Handle EOF (Ctrl+D)
        {
            write(STDOUT_FILENO, "\n", 1);
            // free(line[0]);
            break;
        }
        if (!ft_strcmp(line[0], tmp_delimiter[0])) // Check if delimiter is matched
            break;
        if (ft_strchr(delimiter, '\'') || ft_strchr(delimiter, '\"'))
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
        free(line[0]);
    }
    free(line[0]);
    close(pipefd[1]); // Close write end of the pipe
    signal(SIGINT, sigint_handler); // Restore default SIGINT handler
    free(tmp_delimiter[0]);
    return pipefd[0]; // Return read end of the pipe
}

void    append(t_tree *root, int *in, int *out)
{
    if (root->type == APPEND && root->file_name)
    {
        if (*out != STDOUT_FILENO)
            close(*out); // Close previous output fd
        *out = open(root->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (*out == -1)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(root->file_name, 2);
            ft_putstr_fd(": Permission denied\n", 2);
            exit_status = 1;
        }
    }
}

void    less_and_greater(t_tree *root, int *in , int *out)
{
    if (root->type == LESS && root->file_name)
    {
            if (*in != STDIN_FILENO)
                close(*in); // Close previous input fd
            *in = open(root->file_name, O_RDONLY);
            if (*in == -1)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(root->file_name, 2);
                ft_putstr_fd(": No such file or directory\n", 2);
                exit_status = 1;
            }
    }
    else if (root->type == GREATER && root->file_name)
    {
            if (*out != STDOUT_FILENO)
                close(*out); // Close previous output fd
            *out = open(root->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (*out == -1)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(root->file_name, 2);
                ft_putstr_fd(": Permission denied\n", 2);
                exit_status = 1;
            }
    }
}

t_tree *handle_redirections(t_tree *root, int *in, int *out, char **env)
{
    if (!root)
        return NULL;
    // If this is a command node, return it directly
    if (root->type == COMMAND)
        return root;
    
    // Handle redirection nodes
    if (root->type == LESS || root->type == GREATER || 
        root->type == APPEND || root->type == HEREDOC)
    {
        // First, recursively process the left subtree to find more redirections
        t_tree *cmd = handle_redirections(root->left, in, out, env);
        if (root->file_name && !root->file_name[0])
        {
            ft_putstr_fd("minishell : ambigious redirection\n", 2);
            exit_status = 1;
            // free(root->file_name);
            return (NULL);
        }
        if (!cmd && exit_status == 1)
            return NULL;
        // Then handle this redirection
        if ((root->type == GREATER || root->type == LESS )&& root->file_name)
        {
            less_and_greater(root, in ,out);
            if (*in == -1 || *out == -1)
                return (NULL);
            return(cmd);
        }
        else if (root->type == APPEND && root->file_name)
        {
           append(root, in , out);
           if (*in == -1 || *out == -1)
                return (NULL);
           return(cmd);
        }
        else if (root->type == HEREDOC && root->file_name)
        {
            if (*in != STDIN_FILENO)
                close(*in); // Close previous input fd
            *in = handle_heredoc(root->file_name, env);
            if (*in == -1)
            {
                perror("heredoc failed");
                exit_status = 1;
                return NULL;
            }
        }
          return cmd;
    }
    return handle_redirections(root->left, in, out, env);
}