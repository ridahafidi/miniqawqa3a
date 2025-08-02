/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:29:36 by yel-qori          #+#    #+#             */
/*   Updated: 2025/08/02 18:46:22 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	should_expand_token(char *token, int *is_delimiter)
{
	if (!ft_strcmp(token, "<<"))
	{
		*is_delimiter = 1;
		return (0);
	}
	else if (*is_delimiter)
	{
		*is_delimiter = 0;
		return (0);
	}
	return (1);
}

char	*process_token_expansion(char *token, char **env, int exit_status)
{
	char	*expanded_token;

	expanded_token = expand_string(token, env, exit_status, 0);
	if (expanded_token != token)
	{
		free(token);
		return (expanded_token);
	}
	return (token);
}

int	process_single_token(char *input, int *i, t_token_params *params,
		t_token_data *data)
{
	int	start;
	int	end;

	start = *i;
	end = find_token_end(input, start);
	data->tokens[data->token_count] = create_token(input, start, end);
	if (!data->tokens[data->token_count])
	{
		free_tokens_array(data->tokens, data->token_count);
		return (0);
	}
	if (should_expand_token(data->tokens[data->token_count],
			&data->is_delimiter))
		data->tokens[data->token_count] = process_token_expansion(
				data->tokens[data->token_count],
				params->env, params->exit_status);
	data->token_count++;
	*i = end;
	return (1);
}

t_token_data	*init_token_data(void)
{
	t_token_data	*data;

	data = malloc(sizeof(t_token_data));
	if (!data)
		return (NULL);
	data->tokens = malloc(sizeof(char *) * 1000);
	if (!data->tokens)
	{
		free(data);
		return (NULL);
	}
	data->token_count = 0;
	data->is_delimiter = 0;
	return (data);
}

char	**initial_tokenization_with_env(char *input, char **env,
		int exit_status)
{
	t_token_data	*data;
	char			**result;
	int				i;
	t_token_params	params;

	params.env = env;
	params.exit_status = exit_status;
	data = init_token_data();
	if (!data)
		return (NULL);
	i = 0;
	while (input[i])
	{
		skip_whitespace(input, &i);
		if (!input[i])
			break ;
		if (!process_single_token(input, &i, &params, data))
			return (NULL);
	}
	data->tokens[data->token_count] = NULL;
	result = data->tokens;
	free(data);
	return (result);
}
