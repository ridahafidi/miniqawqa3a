/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:17:18 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/06 19:48:18 by rhafidi          ###   ########.fr       */
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
			free_array(dir);
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

void    execute_command(t_tree *root, int in, int out, char **env)
{
    char *path;
    (void)in;
    (void)out;
    
    int i = 0;
    // First check if it's an absolute or relative path
    while(root->command[i][0] == '\0')
    {
        i++;
        if (!root->command[i])
            exit(EXIT_SUCCESS);
    }
    if (ft_strchr(root->command[i], '/') || root->command[i][0] == '.')
    {
        if (access(root->command[i], F_OK) == -1)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(root->command[i], STDERR_FILENO);
            ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
            exit(127);  // Command not found
        }
        if (access(root->command[i], X_OK) == -1)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(root->command[i], STDERR_FILENO);
            ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
            exit(126);  // Permission denied
        }
        path = root->command[i];
    }
    else
    {
        path = get_path(root->command[i], env);
        if (!path)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(root->command[i], STDERR_FILENO);
            ft_putstr_fd(": command not found\n", STDERR_FILENO);
            exit(127);  // Command not found
        }
    }
    
    if (execve(path, &root->command[i], env) == -1)
    {
        if (errno == EACCES)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(root->command[i], STDERR_FILENO);
            ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
            exit(126);  // Permission denied
        }
        if (errno == ENOENT)
            exit(127);  // No such file or directory
        exit(EXIT_FAILURE);     // Other execution errors
    }
}
