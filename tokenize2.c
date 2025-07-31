/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:28:52 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/25 15:28:58 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens_array(char **tokens, int token_count)
{
	int	j;

	j = 0;
	while (j < token_count)
	{
		free(tokens[j]);
		j++;
	}
	free(tokens);
}

void	skip_whitespace(char *input, int *i)
{
	while (input[*i] && input[*i] == ' ')
		(*i)++;
}

void	handle_quotes_parsing(char input_char, char *quote, int *quote_type)
{
	if (input_char == '\'' && !*quote)
	{
		*quote = input_char;
		*quote_type = 0;
	}
	else if (input_char == '"' && !*quote)
	{
		*quote = input_char;
		*quote_type = 1;
	}
	else if (input_char == *quote)
		*quote = 0;
}

int	find_token_end(char *input, int start)
{
	int		i;
	char	quote;
	int		quote_type;

	i = start;
	quote = 0;
	quote_type = -1;
	while (input[i])
	{
		handle_quotes_parsing(input[i], &quote, &quote_type);
		if (!quote && input[i] == ' ')
			break ;
		i++;
	}
	return (i);
}

char	*create_token(char *input, int start, int end)
{
	char	*token;
	int		len;

	len = end - start;
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	strncpy(token, input + start, len);
	token[len] = '\0';
	return (token);
}
