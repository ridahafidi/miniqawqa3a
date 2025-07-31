/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:30:34 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/25 15:31:12 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_input_format(char *input)
{
	if (!input || !*input)
		return (0);
	if (!check_valid_quotes(input))
	{
		printf("minishell: unclosed quotes\n");
		return (0);
	}
	if (special_characters(input))
		return (0);
	return (1);
}

char	*prepare_input_with_spaces(char *input)
{
	char	*spaced_input;

	spaced_input = add_delimiter_spaces(input);
	if (!spaced_input)
		return (NULL);
	return (spaced_input);
}

char	**perform_tokenization(char *spaced_input, char **env, int exit_status)
{
	char	**tokens;

	tokens = initial_tokenization_with_env(spaced_input, env, exit_status);
	return (tokens);
}

int	validate_syntax_and_pipes(char **tokens, int *exit_status)
{
	if (check_syntax_errors(tokens))
	{
		*exit_status = 2;
		return (0);
	}
	if (!tokens)
		return (0);
	if (!invalid_pipe(tokens))
	{
		free_token_array(tokens);
		return (0);
	}
	return (1);
}

char	**tokenize_input(char *input, char **env, int exit_status)
{
	char	**tokens;
	char	*spaced_input;

	if (!validate_input_format(input))
		return (NULL);
	spaced_input = prepare_input_with_spaces(input);
	if (!spaced_input)
		return (NULL);
	tokens = perform_tokenization(spaced_input, env, exit_status);
	free(spaced_input);
	if (!validate_syntax_and_pipes(tokens, &exit_status))
		return (NULL);
	return (tokens);
}
