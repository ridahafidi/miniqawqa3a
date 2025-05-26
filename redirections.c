/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:39:56 by rhafidi           #+#    #+#             */
/*   Updated: 2025/05/26 15:35:36 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    append(t_tree *root, int *in, int *out,int flag)
{
    if (root->type == APPEND && root->file_name)
    {
        if (flag)
        {
            int fd = open(root->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
            fprintf(stdout, "filename = %d\n fd = %s\n", fd, root->file_name);
            if (fd == -1)
            {
                perror("opening input file failed");
                exit(EXIT_FAILURE);
            }
            return ;
        }
        *out = open(root->file_name, O_WRONLY | O_CREAT | O_APPEND , 0644);
        fprintf(stdout, "filename = %d\n fd = %s\n", *out, root->file_name);
        if (*out == -1)
        {
            perror("opening output file failed");
            exit(1);
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
            fprintf(stdout, "filename = %d\n fd = %s\n", fd, root->file_name);
            if (fd == -1)
            {
                perror("opening input file failed");
                exit(EXIT_FAILURE);
            }
            return ;
        }
        *in = open(root->file_name, O_RDONLY);
        fprintf(stdout, "filename = %d\n fd = %s\n", *in, root->file_name);
        if (*in == -1)
        {
            perror("opening input file failed :");
            exit(1);
        }
    }
    else if (root->type == GREATER && root->file_name)
    {
        if (flag)
        {
            int fd = open(root->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            fprintf(stdout, "filename = %d\n fd = %s\n", fd, root->file_name);
            if (fd == -1)
            {
                perror("opening input file failed");
                exit(EXIT_FAILURE);
            }
            return ;
        }
        *out = open(root->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        fprintf(stdout, "filename = %d\n fd = %s\n", *out, root->file_name);
        if (*out == -1)
        {
            perror("openeing out file failed");
            exit(1);
        }
    }
}

void    handle_redirections(t_tree *root, int *in, int *out, int flag)
{
    if (!root)
        return;
    if ((root->type == LESS || root->type == GREATER) && root->file_name)
        less_and_greater(root, in ,out, flag);
    else if (root->type == APPEND && root->file_name)
        append(root, in, out, flag);
}