/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:49:36 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/31 20:49:58 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **argv, char **env)
{
	int	i;

	i = 0;
	if (!env || argv[1])
		return (EXIT_FAILURE);
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
			ft_putstr_fd(env[i], STDOUT_FILENO);
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
	return (EXIT_SUCCESS);
}

static void	remove_from_env(char *arg, char ***env)
{
	int	len;
	int	j;
	int	n;

	len = 0;
	while ((*env)[len])
		len++;
	j = 0;
	while (j < len)
	{
		n = ft_strlen(arg);
		if (!ft_strncmp(arg, (*env)[j], n) && ((*env)[j][n] == '='
				|| (*env)[j][n] == '\0'))
		{
			ft_swap_ptr(env, j, len - 1);
			free((*env)[len - 1]);
			(*env)[len - 1] = NULL;
			break ;
		}
		j++;
	}
}

int	ft_unset(char **argv, char ***env, char ***exported)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		remove_from_env(argv[i], env);
		remove_from_env(argv[i], exported);
		i++;
	}
	return (EXIT_SUCCESS);
}
