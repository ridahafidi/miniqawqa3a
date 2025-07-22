/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:17:18 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/22 16:19:38 by rhafidi          ###   ########.fr       */
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
			free(tmp_cmd);
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

void    execute_command(t_tree *root, t_fd *fd, char **env, char **exported)
{
    char *path;
    int i = 0;
    
    if(fd)
        free(fd);
    
    // Skip empty command arguments
    while(root->command[i] && root->command[i][0] == '\0')
    {
        i++;
    }
    
    // If no valid command found, exit successfully (bash behavior)
    if (!root->command[i])
    {
        free_array(env);
        free_array(exported);
        free_tree(&root);
        clear_history();
        exit(EXIT_SUCCESS);
    }
    
    // Check if it's an absolute or relative path
    if (ft_strchr(root->command[i], '/') || root->command[i][0] == '.')
    {
        // Check if path exists
        if (access(root->command[i], F_OK) == -1)
        {
            free_array(env);
            free_array(exported);
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(root->command[i], STDERR_FILENO);
            ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
            clear_history();
            free_tree(&root);
            exit(127);  // Command not found
        }
        
        // Check if it's a directory
        if (is_directory(root->command[i]))
        {
            free_array(env);
            free_array(exported);
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(root->command[i], STDERR_FILENO);
            ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
            clear_history();
            free_tree(&root);
            exit(126);  // Permission denied (bash behavior for directories)
        }
        
        // Check execute permission
        if (access(root->command[i], X_OK) == -1)
        {
            free_array(env);
            free_array(exported);
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(root->command[i], STDERR_FILENO);
            ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
            clear_history();
            free_tree(&root);
            exit(126);  // Permission denied
        }
        
        path = root->command[i];
    }
    else
    {
        // Check if command exists in PATH
        path = get_path(root->command[i], env);
        if (!path)
        {
            free_array(env);
            free_array(exported);
            clear_history();
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(root->command[i], STDERR_FILENO);
            ft_putstr_fd(": command not found\n", STDERR_FILENO);
            free_tree(&root);
            exit(127);  // Command not found
        }
        
        // Additional check: if found path is a directory (shouldn't happen normally)
        if (is_directory(path))
        {
            free(path);
            free_array(env);
            free_array(exported);
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(root->command[i], STDERR_FILENO);
            ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
            clear_history();
            free_tree(&root);
            exit(126);
        }
    }
    
    // Execute the command
    if (execve(path, &root->command[i], env) == -1)
    {
        // Clean up path if it was allocated by get_path
        if (path != root->command[i])
            free(path);
            
        if (errno == EACCES)
        {
            free_array(env);
            free_array(exported);
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(root->command[i], STDERR_FILENO);
            ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
            clear_history();
            free_tree(&root);
            exit(126);  // Permission denied
        }
        else if (errno == ENOENT)
        {
            free_array(env);
            free_array(exported);
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(root->command[i], STDERR_FILENO);
            ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
            clear_history();
            free_tree(&root);
            exit(127);  // Command not found
        }
        else if (errno == ENOEXEC)
        {
            free_array(env);
            free_array(exported);
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(root->command[i], STDERR_FILENO);
            ft_putstr_fd(": Exec format error\n", STDERR_FILENO);
            clear_history();
            free_tree(&root);
            exit(126);  // Cannot execute
        }
        else
        {
            free_array(env);
            free_array(exported);
            clear_history();
            free_tree(&root);
            exit(EXIT_FAILURE);  // Other execution errors
        }
    }
}