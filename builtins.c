/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 17:21:18 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 20:24:09 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_builtins(t_tree *root, t_data *data, int *exit_status)
{
	int	exit_code;

	exit_code = *exit_status;
	if (!ft_strcmp(root->command[0], "echo"))
		exit_code = ft_echo(root->command);
	else if (!ft_strcmp(root->command[0], "cd"))
		exit_code = ft_cd(root->command, data->env);
	else if (!ft_strcmp(root->command[0], "pwd"))
		exit_code = ft_pwd();
	else if (!ft_strcmp(root->command[0], "export"))
		exit_code = ft_export(root->command, data->env, data->exported,
				exit_status);
	else if (!ft_strcmp(root->command[0], "unset"))
		exit_code = ft_unset(root->command, data->env, data->exported);
	else if (!ft_strcmp(root->command[0], "env"))
		exit_code = ft_env(root->command, data->env[0]);
	else if (!ft_strcmp(root->command[0], "exit"))
		exit_code = ft_exit(root, data, exit_status);
	return (exit_code);
}
