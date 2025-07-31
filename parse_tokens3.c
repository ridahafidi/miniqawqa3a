/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:41:42 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/25 15:43:14 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*create_and_link_redirection(t_tree *root, char **tokens, int i)
{
	t_tree	*redir_node;

	redir_node = create_redirections(tokens[i], tokens[i + 1]);
	redir_node->left = root;
	return (redir_node);
}

int	process_redirection_token(t_tree **root, char **tokens, int i, int end)
{
	if (has_file_argument(i, end))
	{
		*root = create_and_link_redirection(*root, tokens, i);
		return (i + 2);
	}
	return (-1);
}

t_tree	*process_redirections(t_tree *cmd_node, char **tokens, int start,
		int end)
{
	int		i;
	int		redir_type;
	t_tree	*root;

	root = cmd_node;
	i = start;
	while (i <= end)
	{
		redir_type = is_redirections(tokens[i]);
		if (redir_type)
		{
			i = process_redirection_token(&root, tokens, i, end);
			if (i == -1)
				return (NULL);
		}
		else
			i++;
	}
	return (root);
}

int	initialize_end_index(char **tokens, int end)
{
	if (end == -1)
	{
		end = 0;
		while (tokens[end])
			end++;
		end--;
	}
	return (end);
}

void	cleanup_cmd_args(char **cmd_args, int arg_count)
{
	while (arg_count > 0)
		free(cmd_args[--arg_count]);
	free(cmd_args);
}
