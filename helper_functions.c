/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 16:26:03 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/25 16:26:05 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator_char(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

void	free_token_array(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

int	is_complete_quoted_token(char *token)
{
	int	i;

	if (!token)
		return (0);
	i = 0;
	while (token[i])
	{
		if (token[i] != '\'' && token[i] != '"')
			return (0);
		i++;
	}
	return (1);
}
