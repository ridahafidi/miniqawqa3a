/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:03:07 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/31 18:38:45 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_valid_quotes(char *input)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (input[i])
	{
		if (input[i] == '"' || input[i] == '\'')
		{
			if (quote == 0)
				quote = input[i];
			else if (input[i] == quote)
				quote = 0;
		}
		i++;
	}
	if (quote != 0)
	{
		ft_putstr_fd("minishell: syntax error: unexpected EOF while looking",
			2);
		ft_putstr_fd(" for matching quote\n", 2);
		return (0);
	}
	return (1);
}

int	invalid_pipe(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (is_pipe(tokens[i]))
		{
			if (!tokens[i + 1])
			{
				ft_putstr_fd("minishell: syntax error near unexpected token",
					2);
				ft_putstr_fd(" `|'\n", 2);
				return (0);
			}
		}
		i++;
	}
	return (1);
}

static int	handle_quotes(char *input, int *i, char *quote)
{
	if (input[*i] == '\'' || input[*i] == '"')
	{
		if (!*quote)
			*quote = input[*i];
		else if (*quote == input[*i])
			*quote = 0;
		(*i)++;
		return (1);
	}
	return (0);
}

int	special_characters(char *input)
{
	int		i;
	char	quote;

	quote = 0;
	i = 0;
	while (input[i])
	{
		if (handle_quotes(input, &i, &quote))
			continue ;
		if (!quote && input[i] == ';')
		{
			ft_putstr_fd("minishell: syntax error near unexpected token", 2);
			ft_putstr_fd(" `;'\n", 2);
			return (1);
		}
		i++;
	}
	return (0);
}
