/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utills.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 16:07:52 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 16:08:01 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (0);
	return (S_ISDIR(path_stat.st_mode));
}

void	handle_path_errors(t_tree *root, char **env, char **exported, int i)
{
	free_array(env);
	free_array(exported);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(root->command[i], STDERR_FILENO);
	if (access(root->command[i], F_OK) == -1)
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	else if (is_directory(root->command[i]))
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
	else
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	clear_history();
	free_tree(&root);
}
