/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_spaces.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:42:40 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/25 16:47:35 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	get_current_quote_state(char *input, int pos)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (i < pos)
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			if (quote == 0)
				quote = input[i];
			else if (input[i] == quote)
				quote = 0;
		}
		i++;
	}
	return (quote);
}

int	process_operator_in_result(char *input, char *result, int i, int j)
{
	int	len;

	len = ft_strlen(input);
	if (i + 1 < len && input[i] == input[i + 1])
	{
		j = handle_double_operator(input, result, i, j);
		return (j);
	}
	else
	{
		j = handle_single_operator(input, result, i, j);
		return (j);
	}
}

int	process_single_character(char *input, char *result, int i, int j)
{
	char	quote;

	quote = 0;
	if (i > 0)
		quote = get_current_quote_state(input, i);
	if (!quote && is_operator_char(input[i]))
	{
		j = process_operator_in_result(input, result, i, j);
		return (j);
	}
	else
	{
		result[j++] = input[i];
		return (j);
	}
}

void	fill_result_string(char *input, char *result, int len)
{
	int		i;
	int		j;
	char	quote;

	j = 0;
	quote = 0;
	i = 0;
	while (i < len)
	{
		quote = update_quote_state(input[i], quote);
		if (input[i] == '\'' || input[i] == '"')
		{
			result[j++] = input[i];
			i++;
			continue ;
		}
		j = process_single_character(input, result, i, j);
		if (!quote && is_operator_char(input[i]) && i + 1 < len
			&& input[i] == input[i + 1])
			i++;
		i++;
	}
	result[j] = '\0';
}

char	*add_delimiter_spaces(char *input)
{
	int		len;
	int		new_len;
	char	*result;

	len = ft_strlen(input);
	new_len = calculate_new_length(input, len);
	result = malloc(new_len + 1);
	if (!result)
		return (NULL);
	fill_result_string(input, result, len);
	return (result);
}
