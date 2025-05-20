/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_parsing.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:21:08 by yel-qori          #+#    #+#             */
/*   Updated: 2025/05/19 19:35:36 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int  is_redirections(char *tokens)
{
    if (ft_strcmp(tokens, "<") == 0)
        return (1);
    else if (ft_strcmp(tokens, ">") == 0)
        return (2);
    else if (ft_strcmp(tokens, ">>") == 0)
        return (3);
    else
        return (0);
}

t_tree *create_redirections(char *operator, char *file_name)
{
    t_tree *redir;

    redir = create_command();
    if (ft_strcmp(operator, "<") == 0)
    {
        redir->type = LESS;
        redir->file_name = ft_strdup(file_name); 
    }
    else if (ft_strcmp(operator, ">") == 0)
    {
        redir->type = GREATER;
        redir->file_name = ft_strdup(file_name);
    }
    else if (ft_strcmp(operator, ">>"))
    {
        redir->type = APPEND;
        redir->file_name = ft_strdup(file_name);
    }
    return (redir);
}
