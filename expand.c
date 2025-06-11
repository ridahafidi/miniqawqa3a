/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:32:41 by rhafidi           #+#    #+#             */
/*   Updated: 2025/05/30 17:33:32 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_for_dollar(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

char	*get_var_name(char *str)
{
	int		i;
	int		len;
	char	*var_name;

	i = 1;
	len = 0;
	
	if (str[i] == '?')
		return (ft_strdup("?"));
	
	while (str[i + len] && (ft_isalnum(str[i + len]) || str[i + len] == '_'))
		len++;
	
	if (len == 0)
		return (NULL);
	
	var_name = malloc(sizeof(char) * (len + 1));
	if (!var_name)
		return (NULL);
	
	i = 1;
	len = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		var_name[len++] = str[i++];
	var_name[len] = '\0';
	
	return (var_name);
}

char	*get_var_value(char *var_name, char **env, int status)
{
	int		i;
	char	*value;
	char	*status_str;

	if (!var_name)
		return (ft_strdup("$"));
	
	if (!ft_strcmp(var_name, "?"))
	{
		status_str = ft_itoa(status);
		return (status_str);
	}
	i = compare_var_env(var_name, env);
	if (i >= 0)
	{
		value = ft_substr(env[i], find_start(env[i]), ft_strlen(env[i]));
		return (value);
	}
	return (ft_strdup(""));
}

char	*replace_substr(char *str, int start, int len, char *replacement)
{
	char	*result;
	int		result_len;
	int		i;
	int		j;

	if (!str || !replacement)
		return (NULL);
	
	result_len = ft_strlen(str) - len + ft_strlen(replacement);
	result = malloc(sizeof(char) * (result_len + 1));
	if (!result)
		return (NULL);
	
	i = 0;
	j = 0;
	while (i < start)
		result[j++] = str[i++];
	i = 0;
	while (replacement[i])
		result[j++] = replacement[i++];
	i = start + len;
	while (str[i])
		result[j++] = str[i++];
	result[j] = '\0';
	return (result);
}

int	find_var_end(char *str)
{
	int	i;

	i = 1;
	
	if (str[i] == '?')
		return (2);
	
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	
	if (i == 1)
		return (1);
	
	return (i);
}

char	*expand_string(char *str, char **env, int status)
{
	int		i;
	char	*var_name;
	char	*var_value;
	char	*result;
	int		var_len;
	char 	*temp;
	int		in_single_quotes;
	int		in_double_quotes;

	if (!str)
		return (NULL);

	// For strings starting with double quotes and ending with single quotes, return as is
	if (str[0] == '"' && str[ft_strlen(str) - 1] == '\'')
		return (ft_strdup(str));

	result = ft_strdup(str);
	if (!result)
		return (NULL);

	// If no $ sign, return as is (unless it's a quoted string)
	if (!check_for_dollar(str))
		return (result);

	i = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	
	while (result[i])
	{
		// Handle nested quotes
		if (result[i] == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
			i++;
			continue;
		}
		else if (result[i] == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
			i++;
			continue;
		}
		// Only expand if not in single quotes
		else if (result[i] == '$' && !in_double_quotes)
		{
			var_len = find_var_end(&result[i]);
			var_name = get_var_name(&result[i]);
			var_value = get_var_value(var_name, env, status);
			temp = result;
			result = replace_substr(result, i, var_len, var_value);
			free(temp);
			i += ft_strlen(var_value) - 1;
			free(var_name);
			free(var_value);
		}
		i++;
	}
	
	return (result);
}

char	**expand(char **argv, char **env, int status)
{
	int		i;
	int		len;
	char	**expanded;
	char	*temp;

	if (!argv || !argv[0])
		return (NULL);
	len = 0;
	while (argv[len])
		len++;
	expanded = malloc(sizeof(char *) * (len + 1));
	if (!expanded)
		return (NULL);
	i = 0;
	while (i < len)
	{
		expanded[i] = expand_string(argv[i], env, status);
		i++;
	}
	expanded[i] = NULL;
	return (expanded);
}