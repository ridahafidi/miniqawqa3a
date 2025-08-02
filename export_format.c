/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_format.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:40:04 by yel-qori          #+#    #+#             */
/*   Updated: 2025/08/02 20:22:40 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sort_export(char **export)
{
	int	i;
	int	len;
	int	j;
	int	swapped;

	len = 0;
	while (export[len])
		len++;
	i = 0;
	while (i < len - 1)
	{
		swapped = 0;
		j = 0;
		while (j < len - i - 1)
		{
			if (ft_strcmp(export[j], export[j + 1]) > 0)
			{
				ft_swap(export, j, j + 1);
				swapped = 1;
			}
			j++;
		}
		i++;
	}
}

char	*copy_var(char **export, int i)
{
	char	*variable_name;
	int		len;

	len = 0;
	while (export[i][len])
	{
		if (export[i][len] == '=')
		{
			len++;
			break ;
		}
		len++;
	}
	variable_name = malloc(sizeof(char) * len + 1);
	if (!variable_name)
		return (NULL);
	ft_strlcpy(variable_name, export[i], len + 1);
	return (variable_name);
}

void	handle_quotes(char **export, int i, int j)
{
	char	*value;
	char	*variable_name;
	char	*quote;
	char	*tmp;

	value = ft_strdup(&export[i][j + 1]);
	quote = "\"";
	tmp = ft_strjoin(quote, value);
	free(value);
	value = tmp;
	tmp = ft_strjoin(value, quote);
	free(value);
	value = tmp;
	variable_name = copy_var(export, i);
	free(export[i]);
	export[i] = ft_strjoin(variable_name, value);
	free(variable_name);
	free(value);
}

void	print_format(char **export)
{
	int		i;
	int		j;
	char	*tmp;
	char	*declare;

	i = 0;
	j = 0;
	declare = "declare -x ";
	while (export[i])
	{
		tmp = ft_strjoin(declare, export[i]);
		free(export[i]);
		export[i] = tmp;
		j = 0;
		while (export[i][j])
		{
			if (export[i][j] == '=')
			{
				handle_quotes(export, i, j);
				break ;
			}
			j++;
		}
		i++;
	}
}

int	apply_add(char *new_var, char ***env)
{
	int		len;
	int		i;
	char	**new_env;

	len = 0;
	while ((*env)[len])
		len++;
	new_env = malloc((len + 2) * sizeof(char *));
	if (!new_env)
		return (1);
	i = 0;
	while (i < len)
	{
		new_env[i] = ft_strdup((*env)[i]);
		i++;
	}
	new_env[i] = ft_strdup(new_var);
	new_env[i + 1] = NULL;
	free_array(*env);
	*env = new_env;
	return (EXIT_SUCCESS);
}
