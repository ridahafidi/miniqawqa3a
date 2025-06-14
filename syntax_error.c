/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:03:07 by yel-qori          #+#    #+#             */
/*   Updated: 2025/05/27 16:56:46 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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
		return (0);
	return (1);
}

int invalid_pipe(char **tokens)
{
	int i;

	i = 0;
	while (tokens[i])
	{
		if (is_pipe(tokens[i]))
		{
			if (!tokens[i + 1])
			{
				printf("minishell: syntax error near unexpected token `|`\n");
				return (0);
			}
		}
		i++;
	}
	return (1);
}

int special_characters(char *input)
{
    int i;

    i = 0;
    while (input[i])
    {
        if (input[i] == ';')
        {
            printf("error special character `;`\n");
            return (1);
        }
        i++;
    }
    return (0);
}