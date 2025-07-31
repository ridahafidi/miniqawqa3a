/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:31:32 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/25 15:31:35 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_empty_quotes_case(char *str, int index, t_quote_data *data)
{
	if (index == 0 && is_complete_quoted_token(str))
	{
		data->result[data->j++] = '\'';
		data->result[data->j++] = '\'';
		return (1);
	}
	return (0);
}

void	handle_quote_character(char *str, int *i, t_quote_data *data)
{
	if (!data->current_quote && (str[*i] == '\'' || str[*i] == '"'))
	{
		data->current_quote = str[*i];
		(*i)++;
	}
	else if (data->current_quote && str[*i] == data->current_quote)
	{
		data->current_quote = 0;
		(*i)++;
	}
	else
	{
		data->result[data->j++] = str[(*i)++];
	}
}

t_quote_data	*init_quote_data(int len)
{
	t_quote_data	*data;

	data = malloc(sizeof(t_quote_data));
	if (!data)
		return (NULL);
	data->result = malloc(len + 1);
	if (!data->result)
	{
		free(data);
		return (NULL);
	}
	data->j = 0;
	data->current_quote = 0;
	return (data);
}

char	*remove_quotes_from_string(char *str, int index)
{
	t_quote_data	*data;
	char			*result;
	int				i;

	if (!str)
		return (NULL);
	data = init_quote_data(ft_strlen(str));
	if (!data)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (handle_empty_quotes_case(str, index, data))
			break ;
		handle_quote_character(str, &i, data);
	}
	data->result[data->j] = '\0';
	result = data->result;
	free(data);
	return (result);
}

char	*merge_tokens(char **tokens, int start, int end)
{
	char	*merged;
	char	*tmp;
	int		i;

	merged = ft_strdup("");
	i = start;
	while (i <= end)
	{
		tmp = ft_strjoin(merged, tokens[i]);
		free(merged);
		merged = tmp;
		if (i != end)
		{
			tmp = ft_strjoin(merged, " ");
			free(merged);
			merged = tmp;
		}
		i++;
	}
	return (merged);
}
