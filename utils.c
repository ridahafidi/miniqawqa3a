/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:30:09 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/31 20:31:51 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_swap_ptr(char ***arr, int i, int j)
{
	char	*tmp;

	tmp = (*arr)[i];
	(*arr)[i] = (*arr)[j];
	(*arr)[j] = tmp;
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}

int	is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit"))
		return (0);
	return (1);
}
// echo
// still need to handle echo $variable or echo $?

int	compare_var_env(char *arg, char **env)
{
	int	i;
	int	j;
	int	k;
	int	found;

	i = 0;
	found = 0;
	while (env[i])
	{
		j = 0;
		k = 0;
		found = 0;
		while (arg[j])
		{
			if (env[i][k] == arg[j])
				found++;
			k++;
			j++;
		}
		if (ft_strlen(arg) == found && env[i][k] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	find_start(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '=')
			return (i + 1);
		i++;
	}
	return (0);
}
