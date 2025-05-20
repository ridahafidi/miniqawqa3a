/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_spaces.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:42:40 by yel-qori          #+#    #+#             */
/*   Updated: 2025/05/19 19:36:19 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

char	*add_delimiter_spaces(char *input)
{
	int i = 0;
	int j = 0;
	int len = ft_strlen(input);
	char *buffer;
	int quote = 0;

	buffer = malloc(sizeof(char *) * len + 3);
	if (!buffer)
		return (NULL);
	while (input[i])
	{
		while (input[i] != '\0')
		{
			if (input[i] == '"' || input[i] == '\'')
				{
					if (quote == 0)
						quote = input[i];
					else if (quote == input[i])
						quote = 0;
				}
			if (is_operator(input[i]) && quote == 0)
			{
				if (i > 0 && input[i - 1] != ' ')
					buffer[j++] = ' ';
				buffer[j++] = input[i];
				if (input[i + 1] != '\0' && input[i + 1] != ' ')
					buffer[j++] = ' ';
			}
			else
				buffer[j++] = input[i];
			i++;
		}
		buffer[j] = '\0';
	}
    return (buffer);
}
