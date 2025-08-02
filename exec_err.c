/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_err.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 16:10:50 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 19:17:44 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_execution_error(t_tree *root, char *path, char **env,
		char **exported)
{
	if (path != root->command[0])
		free(path);
	free_array(env);
	free_array(exported);
	clear_history();
	free_tree(&root);
}

void	handle_execve_error(t_tree *root, char *path, char **env,
		char **exported)
{
	int	i;

	i = 0;
	while (root->command[i] && root->command[i][0] == '\0')
		i++;
	handle_execution_error(root, path, env, exported);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(root->command[i], STDERR_FILENO);
	if (errno == EACCES)
	{
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		exit(126);
	}
	else if (errno == ENOENT)
	{
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		exit(127);
	}
	else if (errno == ENOEXEC)
	{
		ft_putstr_fd(": Exec format error\n", STDERR_FILENO);
		exit(126);
	}
	else
		exit(EXIT_FAILURE);
}
