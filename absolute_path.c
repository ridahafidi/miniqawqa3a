/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   absolute_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 16:03:30 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 19:15:57 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_absolute_path(t_tree *root, char **env, char **exported, int i)
{
	if (access(root->command[i], F_OK) == -1)
	{
		handle_path_errors(root, env, exported, i);
		exit(127);
	}
	if (is_directory(root->command[i]))
	{
		handle_path_errors(root, env, exported, i);
		exit(126);
	}
	if (access(root->command[i], X_OK) == -1)
	{
		handle_path_errors(root, env, exported, i);
		exit(126);
	}
	return (root->command[i]);
}
