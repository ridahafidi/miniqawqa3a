/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:17:18 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/01 22:00:44 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_array(char **str)
{
    int i;

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

char    *get_path(char *cmd, char **env)
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

void    access_exec(char **argv, char **env)
{
    if (access(argv[0], F_OK | X_OK) == -1)
    {
        perror("access failed : ");
        free_array(argv);
        exit(1);
    }    
    if (execve(argv[0], argv, env) == -1)
    {
        perror("exec failed :");
        free_array(argv);
        exit(1);
    }
}

int is_directory(const char *path)
{
    struct stat path_stat;
    
    if (stat(path, &path_stat) != 0)
        return (0);
    return (S_ISDIR(path_stat.st_mode));
}


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

 char	*get_command_path(t_tree *root, char **env, char **exported, int i)
{
	if (handle_dot_command(root, env, exported, i))
		return (NULL);
	if (ft_strchr(root->command[i], '/') || root->command[i][0] == '.')
		return (handle_absolute_path(root, env, exported, i));
	else
		return (handle_relative_path(root, env, exported, i));
}

void	execute_command(t_tree *root, t_data *data, int *exit_status)
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
