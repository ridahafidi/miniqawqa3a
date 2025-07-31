/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_parsing1.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:40:04 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/25 15:40:06 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirections(char *tokens)
{
	if (ft_strcmp(tokens, "<") == 0)
		return (1);
	else if (ft_strcmp(tokens, ">") == 0)
		return (2);
	else if (ft_strcmp(tokens, ">>") == 0)
		return (3);
	else if (ft_strcmp(tokens, "<<") == 0)
		return (4);
	else
		return (0);
}

void	set_less_redirection(t_tree *redir, char *file_name)
{
	char	*tmp;

	redir->type = LESS;
	tmp = remove_quotes_from_string(file_name, 1);
	redir->file_name = ft_strdup(tmp);
	free(tmp);
}

void	set_greater_redirection(t_tree *redir, char *file_name)
{
	char	*tmp;

	redir->type = GREATER;
	tmp = remove_quotes_from_string(file_name, 1);
	redir->file_name = ft_strdup(tmp);
	free(tmp);
}

void	set_append_redirection(t_tree *redir, char *file_name)
{
	char	*tmp;

	redir->type = APPEND;
	tmp = remove_quotes_from_string(file_name, 1);
	redir->file_name = ft_strdup(tmp);
	free(tmp);
}
