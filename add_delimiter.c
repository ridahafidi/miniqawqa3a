/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_delimiter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:50:12 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/25 15:50:39 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	update_quote_state(char current_char, char quote)
{
	if (current_char == '\'' || current_char == '"')
	{
		if (!quote)
			return (current_char);
		else if (quote == current_char)
			return (0);
	}
	return (quote);
}

int	process_length_calculation(char *input, int i, int len, char quote)
{
	int	extra_len;

	extra_len = 0;
	if (!quote && is_operator_char(input[i]))
	{
		if (i + 1 < len && input[i] == input[i + 1])
			extra_len = 2;
		else
			extra_len = 2;
	}
	return (extra_len);
}

int	calculate_new_length(char *input, int len)
{
	int		i;
	int		new_len;
	char	quote;

	new_len = len;
	quote = 0;
	i = 0;
	while (i < len)
	{
		quote = update_quote_state(input[i], quote);
		if (input[i] == '\'' || input[i] == '"')
		{
			i++;
			continue ;
		}
		new_len += process_length_calculation(input, i, len, quote);
		if (!quote && is_operator_char(input[i]) && i + 1 < len
			&& input[i] == input[i + 1])
			i++;
		i++;
	}
	return (new_len);
}

int	handle_double_operator(char *input, char *result, int i, int j)
{
	int	new_j;

	new_j = j;
	if (new_j > 0 && result[new_j - 1] != ' ')
		result[new_j++] = ' ';
	result[new_j++] = input[i];
	result[new_j++] = input[i + 1];
	if (i + 2 < ft_strlen(input) && input[i + 2] != ' ')
		result[new_j++] = ' ';
	return (new_j);
}

int	handle_single_operator(char *input, char *result, int i, int j)
{
	int	new_j;

	new_j = j;
	if (new_j > 0 && result[new_j - 1] != ' ')
		result[new_j++] = ' ';
	result[new_j++] = input[i];
	if (i + 1 < ft_strlen(input) && input[i + 1] != ' ')
		result[new_j++] = ' ';
	return (new_j);
}
