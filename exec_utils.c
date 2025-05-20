/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:17:18 by rhafidi           #+#    #+#             */
/*   Updated: 2025/05/16 17:42:43 by rhafidi          ###   ########.fr       */
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
    char **argv;
    char    *path;
    int     status;
    
    argv = root->command;
    if (argv[0][0] == '/' || argv[0][0] == '.')
        access_exec(argv, env);
    path = get_path(argv[0], env);
    if (!path)
    {
        perror("command not found");
        exit(127);
    }
    execve(path, argv, env);
    perror("exec failed");
    free_array(argv);
    exit(1);
}
