/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_parsing.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:21:08 by yel-qori          #+#    #+#             */
/*   Updated: 2025/08/02 19:21:33 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_heredoc_redirection(t_tree *redir, char *file_name)
{
	redir->type = HEREDOC;
	redir->file_name = ft_strdup(file_name);
}

t_tree	*create_redirections(char *operator, char *file_name)
{
	t_tree	*redir;

	redir = create_command();
	if (ft_strcmp(operator, "<") == 0)
		set_less_redirection(redir, file_name);
	else if (ft_strcmp(operator, ">") == 0)
		set_greater_redirection(redir, file_name);
	else if (ft_strcmp(operator, ">>") == 0)
		set_append_redirection(redir, file_name);
	else if (ft_strcmp(operator, "<<") == 0)
		set_heredoc_redirection(redir, file_name);
	return (redir);
}

int	check_redirection(char *token)
{
	if (!token)
		return (0);
	return (ft_strcmp(token, ">") == 0 || ft_strcmp(token, "<") == 0
		|| ft_strcmp(token, ">>") == 0 || ft_strcmp(token, "<<") == 0);
}
