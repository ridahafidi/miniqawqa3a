/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 17:10:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 17:10:20 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_single_quote(char *result, char *str, int *i, int heredoc)
{
	char	*tmp;
	char	curr[2];

	if (heredoc == 1)
	{
		(*i)++;
		return (result);
	}
	curr[0] = str[*i];
	curr[1] = '\0';
	tmp = ft_strjoin(result, curr);
	free(result);
	(*i)++;
	return (tmp);
}

char	*handle_double_quote(char *result, char *str, int *i, int heredoc)
{
	char	*tmp;
	char	curr[2];

	if (heredoc == 1)
	{
		(*i)++;
		return (result);
	}
	curr[0] = str[*i];
	curr[1] = '\0';
	tmp = ft_strjoin(result, curr);
	free(result);
	(*i)++;
	return (tmp);
}

char	*handle_double_dollar(char *result, int *i)
{
	char	*tmp;

	tmp = ft_strjoin(result, "$$");
	free(result);
	(*i) += 2;
	return (tmp);
}

char	*process_single_quote_section(char *result, char *quoted_section)
{
	char	*tmp;

	tmp = ft_strjoin(result, quoted_section);
	free(result);
	return (tmp);
}

char	*process_double_quote_section(char *result, char *quoted_section,
		char **env, int status)
{
	char	*expanded_section;
	char	*tmp;

	expanded_section = expand_string(quoted_section, env, status, 0);
	tmp = ft_strjoin(result, expanded_section);
	free(result);
	free(expanded_section);
	return (tmp);
}
