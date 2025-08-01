/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_env_ops.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:46:56 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/31 21:20:21 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	create_new_append_env(char *var, char ***env, int len,
		int var_name_len)
{
	char	*new_var;
	char	**new_env;
	int		i;

	new_var = build_append_var(var, var_name_len);
	new_env = malloc(sizeof(char *) * (len + 2));
	i = 0;
	while (i < len)
	{
		new_env[i] = (*env)[i];
		i++;
	}
	new_env[len] = new_var;
	new_env[len + 1] = NULL;
	if (*env)
		free(*env);
	*env = new_env;
}

void	handle_append_env(char *var, char ***env, int len, int var_name_len)
{
	int		i;
	char	*old_var;
	char	*append_value;

	append_value = var + var_name_len + 2;
	i = 0;
	while (i < len)
	{
		if (!ft_strncmp((*env)[i], var, var_name_len)
			&& (*env)[i][var_name_len] == '=')
		{
			old_var = (*env)[i];
			(*env)[i] = ft_strjoin(old_var, append_value);
			free(old_var);
			return ;
		}
		i++;
	}
	create_new_append_env(var, env, len, var_name_len);
}

int	check_append_operation(char *var)
{
	int	var_name_len;

	var_name_len = 0;
	while (var[var_name_len])
	{
		if (var[var_name_len] == '+' && var[var_name_len + 1] == '=')
			return (var_name_len);
		if (var[var_name_len] == '=')
			break ;
		var_name_len++;
	}
	return (-1);
}

static int	get_eqpos(char *var)
{
	int	eqpos;

	eqpos = 0;
	while (var[eqpos] && var[eqpos] != '=')
		eqpos++;
	return (eqpos);
}

void	handle_regular_assignment(char *var, char ***env, int len)
{
	int		eqpos;
	int		i;
	char	**new_env;

	eqpos = get_eqpos(var);
	i = 0;
	while (i < len)
	{
		if (!ft_strncmp((*env)[i], var, eqpos) && (*env)[i][eqpos] == '=')
		{
			free((*env)[i]);
			(*env)[i] = ft_strdup(var);
			return ;
		}
		i++;
	}
	new_env = malloc(sizeof(char *) * (len + 2));
	i = -1;
	while (++i < len)
		new_env[i] = (*env)[i];
	new_env[len] = ft_strdup(var);
	new_env[len + 1] = NULL;
	if (*env)
		free(*env);
	*env = new_env;
}
