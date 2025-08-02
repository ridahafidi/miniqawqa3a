/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:41:57 by yel-qori          #+#    #+#             */
/*   Updated: 2025/08/02 19:20:48 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_redirection_syntax(char **tokens, int i)
{
	if (!tokens[i + 1] || is_pipe(tokens[i + 1]) || is_redirection(tokens[i
				+ 1]))
	{
		print_syntax_error(tokens[i + 1]);
		return (1);
	}
	return (0);
}

int	check_pipe_syntax(char **tokens, int i)
{
	if (!tokens[i + 1] || is_pipe(tokens[i + 1]))
	{
		print_syntax_error(tokens[i + 1]);
		return (1);
	}
	return (0);
}

int	check_last_token_pipe(char **tokens, int i)
{
	if (i > 0 && is_pipe(tokens[i - 1]))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (1);
	}
	return (0);
}

int	check_syntax_errors(char **tokens)
{
	int	i;

	if (!tokens || !tokens[0])
		return (0);
	if (check_first_token_pipe(tokens))
		return (1);
	i = 0;
	while (tokens[i])
	{
		if (is_redirection(tokens[i]))
		{
			if (check_redirection_syntax(tokens, i))
				return (1);
		}
		else if (is_pipe(tokens[i]))
		{
			if (check_pipe_syntax(tokens, i))
				return (1);
		}
		i++;
	}
	if (check_last_token_pipe(tokens, i))
		return (1);
	return (0);
}

void	strip_quotes_from_command(t_tree *ast)
{
	int		i;
	char	*new_str;

	i = 0;
	while (ast->command[i])
	{
		new_str = remove_quotes_from_string(ast->command[i], i);
		if (new_str)
		{
			free(ast->command[i]);
			ast->command[i] = new_str;
		}
		i++;
	}
}
