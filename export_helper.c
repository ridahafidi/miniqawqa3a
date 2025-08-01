/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:45:32 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/31 21:21:31 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_to_new_exported(char **new_exported, char ***exported, int len,
		int i)
{
	while (i < len)
	{
		new_exported[i] = (*exported)[i];
		i++;
	}
}

char	*build_append_var(char *var, int var_name_len)
{
	char	*var_name;
	char	*temp;
	char	*new_var;

	var_name = ft_substr(var, 0, var_name_len);
	temp = ft_strjoin(var_name, "=");
	new_var = ft_strjoin(temp, var + var_name_len + 2);
	free(var_name);
	free(temp);
	return (new_var);
}

static void	create_new_append_var(char *var, char ***exported, int len)
{
	int		i;
	int		var_name_len;
	char	*new_var;
	char	**new_exported;

	var_name_len = 0;
	while (var[var_name_len] && var[var_name_len] != '+')
		var_name_len++;
	new_var = build_append_var(var, var_name_len);
	new_exported = malloc(sizeof(char *) * (len + 2));
	i = 0;
	copy_to_new_exported(new_exported, exported, len, i);
	new_exported[len] = new_var;
	new_exported[len + 1] = NULL;
	if (*exported)
		free(*exported);
	*exported = new_exported;
}

static void	add_new_exported(char *var, char ***exported, int len)
{
	char	**new_exported;
	int		i;

	new_exported = malloc(sizeof(char *) * (len + 2));
	i = 0;
	while (i < len)
	{
		new_exported[i] = (*exported)[i];
		i++;
	}
	new_exported[len] = ft_strdup(var);
	new_exported[len + 1] = NULL;
	if (*exported)
		free(*exported);
	*exported = new_exported;
}

void	add_or_update_exported(char *var, char ***exported)
{
	int	idx;
	int	len;

	idx = is_in_exported(var, *exported);
	len = 0;
	while (*exported && (*exported)[len])
		len++;
	if (idx >= 0)
	{
		free((*exported)[idx]);
		(*exported)[idx] = ft_strdup(var);
	}
	else if (idx == -2)
	{
		if (!handle_append_exported(var, exported, len))
			create_new_append_var(var, exported, len);
	}
	else
		add_new_exported(var, exported, len);
}
