/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   relative_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 16:05:34 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 19:21:46 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_command_not_found(t_tree *root, char **env, char **exported,
		int i)
{
	free_array(env);
	free_array(exported);
	clear_history();
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(root->command[i], STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	free_tree(&root);
}

char	*handle_relative_path(t_tree *root, char **env, char **exported, int i)
{
	char	*path;

	path = get_path(root->command[i], env);
	if (!path)
	{
		handle_command_not_found(root, env, exported, i);
		exit(127);
	}
	if (is_directory(path))
	{
		free(path);
		handle_path_errors(root, env, exported, i);
		exit(126);
	}
	return (path);
}
