/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:52:23 by yel-qori          #+#    #+#             */
/*   Updated: 2025/08/02 20:22:47 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**collect_command_arguments(char **tokens, int start, int end)
{
	char	**cmd_args;

	cmd_args = malloc(sizeof(char *) * (end - start + 2));
	if (!cmd_args)
		return (NULL);
	return (process_tokens_loop(cmd_args, tokens, start, end));
}

t_tree	*parse_token_subset(char **tokens, int start, int end)
{
	t_tree	*cmd_node;
	char	**cmd_args;

	end = initialize_end_index(tokens, end);
	if (start > end || !tokens[start])
		return (NULL);
	cmd_args = collect_command_arguments(tokens, start, end);
	if (!cmd_args)
		return (NULL);
	cmd_node = create_command_node(cmd_args);
	if (!cmd_node)
	{
		free(cmd_args);
		return (NULL);
	}
	cmd_node = process_redirections(cmd_node, tokens, start, end);
	strip_quotes_from_ast(cmd_node);
	return (cmd_node);
}

t_tree	*create_pipe_node(void)
{
	t_tree	*cmd;

	cmd = create_command();
	if (!cmd)
		return (NULL);
	cmd->type = PIPE;
	return (cmd);
}

t_tree	*parse_tokens(char **tokens)
{
	int		i;
	int		start;
	t_tree	*pipe_node;

	start = 0;
	i = 0;
	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], "|") == 0)
		{
			pipe_node = create_pipe_node();
			pipe_node->left = parse_token_subset(tokens, start, i - 1);
			start = i + 1;
			pipe_node->right = parse_tokens(tokens + start);
			return (pipe_node);
		}
		i++;
	}
	return (parse_token_subset(tokens, 0, -1));
}
