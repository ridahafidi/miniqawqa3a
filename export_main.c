/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:48:22 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/31 20:48:50 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_or_update_env(char *var, char ***env)
{
	int	len;
	int	var_name_len;

	if (!ft_strchr(var, '='))
		return ;
	len = 0;
	while (*env && (*env)[len])
		len++;
	var_name_len = check_append_operation(var);
	if (var_name_len >= 0)
		handle_append_env(var, env, len, var_name_len);
	else
		handle_regular_assignment(var, env, len);
}

void	add_var(char **argv, char ***env, char ***exported, int *exit_status)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
			*exit_status = 1;
		}
		else
		{
			if (ft_strchr(argv[i], '='))
			{
				add_or_update_exported(argv[i], exported);
				add_or_update_env(argv[i], env);
			}
			else
				add_or_update_exported(argv[i], exported);
			*exit_status = 0;
		}
		i++;
	}
}

static void	print_exported_vars(char **copy)
{
	int	i;

	i = 0;
	while (copy[i])
	{
		ft_putstr_fd(copy[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
}

static void	free_copy_array(char **copy)
{
	int	i;

	i = 0;
	while (copy[i])
	{
		free(copy[i]);
		i++;
	}
	free(copy);
}

int	ft_export(char **argv, char ***env, char ***exported, int *exit_status)
{
	char	**copy;

	if (!argv[1])
	{
		copy = copy_env(*exported);
		sort_export(copy);
		print_format(copy);
		print_exported_vars(copy);
		free_copy_array(copy);
		return (EXIT_SUCCESS);
	}
	else
	{
		add_var(argv, env, exported, exit_status);
		return (*exit_status);
	}
	return (EXIT_FAILURE);
}
