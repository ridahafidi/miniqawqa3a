/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:17:18 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 20:19:13 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		str[i] = NULL;
		i++;
	}
	free(str);
	str = NULL;
}

char	*get_env_path(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (&env[i][5]);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, char **env)
{
	char	*path;
	char	**dir;
	char	*cmd_path;
	char	*tmp_cmd;
	int		i;

	i = 0;
	path = get_env_path(env);
	if (!path)
		return (NULL);
	dir = ft_split(path, ':');
	tmp_cmd = ft_strjoin("/", cmd);
	while (dir[i])
	{
		cmd_path = ft_strjoin(dir[i], tmp_cmd);
		if (!access(cmd_path, F_OK | X_OK))
		{
			(free_array(dir), free(tmp_cmd));
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	(free(tmp_cmd), free_array(dir));
	return (NULL);
}

char	*get_command_path(t_tree *root, char **env, char **exported, int i)
{
	if (handle_dot_command(root, env, exported, i))
		return (NULL);
	if (ft_strchr(root->command[i], '/') || root->command[i][0] == '.')
		return (handle_absolute_path(root, env, exported, i));
	else
		return (handle_relative_path(root, env, exported, i));
}

void	execute_command(t_tree *root, t_data *data)
{
	char	*path;
	int		i;
	char	**env;
	char	**exported;

	i = 0;
	if (data->fds)
		free(data->fds);
	env = data->env[0];
	exported = data->exported[0];
	while (root->command[i] && root->command[i][0] == '\0')
		i++;
	if (!root->command[i])
	{
		free_array(env);
		free_array(exported);
		free_tree(&root);
		free(data);
		clear_history();
		exit(EXIT_SUCCESS);
	}
	path = get_command_path(root, env, exported, i);
	free(data);
	if (execve(path, &root->command[i], env) == -1)
		handle_execve_error(root, path, env, exported);
}
