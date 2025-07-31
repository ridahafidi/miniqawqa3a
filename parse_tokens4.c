/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:41:35 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/25 15:41:39 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirection_skip(int i, int end, char **cmd_args, int arg_count)
{
	i += 2;
	if (i > end + 1)
	{
		cleanup_cmd_args(cmd_args, arg_count);
		return (-1);
	}
	return (i);
}

int	add_argument_to_array(char **cmd_args, char **tokens, int i, int arg_count)
{
	cmd_args[arg_count] = ft_strdup(tokens[i]);
	if (!cmd_args[arg_count])
	{
		cleanup_cmd_args(cmd_args, arg_count);
		return (-1);
	}
	return (0);
}

char	**process_tokens_loop(char **cmd_args, char **tokens, int start,
		int end)
{
	int	i;
	int	arg_count;
	int	result;

	arg_count = 0;
	i = start;
	while (i <= end)
	{
		if (is_redirections(tokens[i]))
		{
			result = handle_redirection_skip(i, end, cmd_args, arg_count);
			if (result == -1)
				return (NULL);
			i = result;
			continue ;
		}
		if (add_argument_to_array(cmd_args, tokens, i, arg_count) == -1)
			return (NULL);
		arg_count++;
		i++;
	}
	cmd_args[arg_count] = NULL;
	return (cmd_args);
}
