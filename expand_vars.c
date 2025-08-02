/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 17:05:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 19:19:52 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_braced_var_name(char *str, int *i, int *len)
{
	char	*var_name;

	(*i)++;
	while (str[*i + *len] && str[*i + *len] != '}')
		(*len)++;
	if (str[*i + *len] != '}')
		return (NULL);
	var_name = malloc(sizeof(char) * (*len + 1));
	if (!var_name)
		return (NULL);
	*len = 0;
	while (str[*i] && str[*i] != '}')
		var_name[(*len)++] = str[(*i)++];
	var_name[*len] = '\0';
	return (var_name);
}

char	*get_regular_var_name(char *str, int *i, int *len)
{
	char	*var_name;

	while (str[*i + *len] && (ft_isalnum(str[*i + *len]) || str[*i
				+ *len] == '_'))
		(*len)++;
	if (*len == 0)
		return (NULL);
	var_name = malloc(sizeof(char) * (*len + 1));
	if (!var_name)
		return (NULL);
	*i = 1;
	*len = 0;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		var_name[(*len)++] = str[(*i)++];
	var_name[*len] = '\0';
	return (var_name);
}

char	*get_var_name(char *str)
{
	int	i;
	int	len;

	i = 1;
	len = 0;
	if (str[i] == '?')
		return (ft_strdup("?"));
	if (str[i] == '$')
		return (ft_strdup("$"));
	if (str[i] == '{')
		return (get_braced_var_name(str, &i, &len));
	return (get_regular_var_name(str, &i, &len));
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
	if (!ft_strcmp(var_name, "$"))
	{
		return (ft_strdup("$"));
	}
	i = compare_var_env(var_name, env);
	if (i >= 0)
	{
		value = ft_substr(env[i], find_start(env[i]), ft_strlen(env[i]));
		return (value);
	}
	return (ft_strdup(""));
}

char	*handle_literal_dollar(char *result, int *i)
{
	char	*tmp;

	tmp = ft_strjoin(result, "$");
	free(result);
	(*i)++;
	return (tmp);
}
