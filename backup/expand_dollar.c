/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 17:15:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 17:02:08 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_dollar_quote(char *result, char *str, int *i, char **env, int status)
{
	char	quote_char;
	int		j;
	int		total_len;
	char	*quoted_section;
	char	*tmp;

	quote_char = str[*i + 1];
	j = *i + 2;
	while (str[j] && str[j] != quote_char)
		j++;
	if (str[j] == quote_char)
	{
		total_len = j - *i + 1;
		quoted_section = ft_substr(str, *i + 1, total_len - 1);
		if (quote_char == '\'')
			result = process_single_quote_section(result, quoted_section);
		else
			result = process_double_quote_section(result, quoted_section, env, status);
		free(quoted_section);
		*i = j + 1;
		return (result);
	}
	else
	{
		tmp = ft_strjoin(result, "$");
		free(result);
		(*i)++;
		return (tmp);
	}
}

char	*handle_exit_status(char *result, int *i, int status)
{
	char	*tmp;
	char	*status_str;
	int		real_status;

	if (WIFEXITED(status))
		real_status = WEXITSTATUS(status);
	else
		real_status = status;
	status_str = ft_itoa(real_status);
	tmp = ft_strjoin(result, status_str);
	free(status_str);
	free(result);
	*i += 2;
	return (tmp);
}

char	*handle_braced_var(char *result, char *str, int *i, char **env,
	int status)
{
	int		start;
	int		j;
	char	*var_name;
	char	*var_value;
	char	*tmp;

	start = *i + 2;
	j = start;
	while (str[j] && str[j] != '}')
		j++;
	if (str[j] == '}' && j > start)
	{
		var_name = ft_substr(str, start, j - start);
		var_value = get_var_value(var_name, env, status);
		free(var_name);
		if (var_value)
		{
			tmp = ft_strjoin(result, var_value);
			free(result);
			result = tmp;
			free(var_value);
		}
		*i = j + 1;
		return (result);
	}
	else
	{
		tmp = ft_strjoin(result, "$");
		free(result);
		(*i)++;
		return (tmp);
	}
}

char	*handle_regular_var(char *result, char *str, int *i, char **env, int status)
{
	int		start;
	int		len;
	char	*var_name;
	char	*var_value;
	char	*tmp;

	start = *i + 1;
	(*i)++;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	len = *i - start;
	var_name = ft_substr(str, start, len);
	var_value = get_var_value(var_name, env, status);
	free(var_name);
	if (var_value)
	{
		tmp = ft_strjoin(result, var_value);
		free(result);
		result = tmp;
		free(var_value);
	}
	return (result);
}

char	*handle_dollar_cases(char *result, char *str, int *i, char **env, int status, int *doll)
{
	if (str[*i + 1] == '$')
	{
		result = handle_double_dollar(result, i);
		return (result);
	}
	if (str[*i + 1] == '\'' || str[*i + 1] == '"')
		return (handle_dollar_quote(result, str, i, env, status));
	if (str[*i + 1] == '?')
		return (handle_exit_status(result, i, status));
	if (str[*i + 1] == '{')
		return (handle_braced_var(result, str, i, env, status));
	if (ft_isalpha(str[*i + 1]) || str[*i + 1] == '_')
		return (handle_regular_var(result, str, i, env, status));
	return (handle_literal_dollar(result, i));
}
