/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:41:14 by yel-qori          #+#    #+#             */
/*   Updated: 2025/08/02 20:22:33 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '=')
		i++;
	return (i);
}

int	indetical_variable(char ***env, char *var)
{
	int	j;

	j = 0;
	while (env[0][j])
	{
		if (!ft_strncmp(var, env[0][j], find_equal(env[0][j])))
		{
			if (var[find_equal(var) - 1] == '+')
			{
				env[0][j] = ft_strjoin(env[0][j], var + find_equal(var) + 1);
				return (0);
			}
			free(env[0][j]);
			env[0][j] = ft_strdup(var);
			return (0);
		}
		j++;
	}
	return (1);
}

int	is_in_exported(char *var, char **exported)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (var[len] && var[len] != '=' && var[len] != '+')
		len++;
	if (var[len] == '+' && var[len + 1] == '=')
		return (-2);
	while (exported && exported[i])
	{
		if (!ft_strncmp(exported[i], var, len) && (exported[i][len] == '\0'
			|| exported[i][len] == '='))
			return (i);
		i++;
	}
	return (-1);
}

static char	*create_appended_var(char *old_var, char *append_value,
		int var_name_len)
{
	char	*new_var;
	char	*temp;

	if (old_var[var_name_len] == '=')
		new_var = ft_strjoin(old_var, append_value);
	else
	{
		temp = ft_strjoin(old_var, "=");
		new_var = ft_strjoin(temp, append_value);
		free(temp);
	}
	return (new_var);
}

int	handle_append_exported(char *var, char ***exported, int len)
{
	int		i;
	int		var_name_len;
	char	*old_var;
	char	*append_value;

	var_name_len = 0;
	while (var[var_name_len] && var[var_name_len] != '+')
		var_name_len++;
	append_value = var + var_name_len + 2;
	i = 0;
	while (i < len)
	{
		if (!ft_strncmp((*exported)[i], var, var_name_len)
			&& ((*exported)[i][var_name_len] == '='
				|| (*exported)[i][var_name_len] == '\0'))
		{
			old_var = (*exported)[i];
			(*exported)[i] = create_appended_var(old_var, append_value,
					var_name_len);
			free(old_var);
			return (1);
		}
		i++;
	}
	return (0);
}
