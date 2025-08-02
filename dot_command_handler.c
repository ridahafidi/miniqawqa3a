/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dot_command_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:00:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 19:17:36 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_single_dot(const char *cmd)
{
	return (cmd[0] == '.' && cmd[1] == '\0');
}

static int	is_only_dots(const char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] != '.')
			return (0);
		i++;
	}
	return (i > 0);
}

void	print_cmdnfound(char *cmd)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
}

int	handle_dot_command(t_tree *root, char **env, char **exported, int i)
{
	char	*cmd;

	cmd = root->command[i];
	if (is_single_dot(cmd))
	{
		free_array(env);
		free_array(exported);
		ft_putstr_fd("minishell: .: filename argument required\n",
			STDERR_FILENO);
		ft_putstr_fd(".: usage: . filename [arguments]\n", STDERR_FILENO);
		clear_history();
		free_tree(&root);
		exit(2);
	}
	else if (is_only_dots(cmd))
	{
		free_array(env);
		free_array(exported);
		print_cmdnfound(cmd);
		clear_history();
		free_tree(&root);
		exit(127);
	}
	return (0);
}
