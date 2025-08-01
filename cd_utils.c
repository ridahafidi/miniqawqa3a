/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:33:27 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/31 21:26:17 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getenv(char **env, const char *name)
{
	int		i;
	size_t	len;

	if (!env || !name)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], name, len) && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

int	check_dir_x(struct stat *file_stat, char *path)
{
	if (stat(path, file_stat) == -1)
	{
		return (0);
	}
	return ((file_stat->st_mode & S_IXUSR) && (S_ISDIR(file_stat->st_mode)));
}

void	update_env(char ***env, char *new_cwd, char *cwd)
{
	int		i;
	char	*tmp;
	char	*joiner;

	i = 0;
	while (env[0][i])
	{
		tmp = env[0][i];
		if (!ft_strncmp("PWD=", env[0][i], ft_strlen("PWD=")))
		{
			free(env[0][i]);
			joiner = ft_strjoin("PWD=", new_cwd);
			env[0][i] = joiner;
			joiner = NULL;
		}
		else if (!ft_strncmp("OLDPWD=", env[0][i], ft_strlen("OLDPWD=")))
		{
			free(env[0][i]);
			joiner = ft_strjoin("OLDPWD=", cwd);
			env[0][i] = joiner;
			joiner = NULL;
		}
		i++;
	}
}

int	count_arguments(char **argv)
{
	int	arg_count;

	arg_count = 0;
	while (argv[arg_count])
		arg_count++;
	return (arg_count);
}

void	print_cd_error(char *path, int error_type)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	if (error_type == 1)
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	else if (error_type == 2)
		ft_putstr_fd(": Not a directory\n", STDERR_FILENO);
	else
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
}
