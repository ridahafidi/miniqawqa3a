/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_handler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:34:32 by yel-qori          #+#    #+#             */
/*   Updated: 2025/08/02 20:16:56 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	change_to_home(char ***env)
{
	char		*path;
	struct stat	file_stat;

	path = ft_getenv(*env, "HOME");
	if (!path)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
		return (1);
	}
	if (check_dir_x(&file_stat, path))
	{
		if (chdir(path) == -1)
			return (print_cd_error(path, 3), 1);
	}
	else
	{
		if (access(path, F_OK) == -1)
			print_cd_error(path, 1);
		else
			print_cd_error(path, 2);
		return (1);
	}
	return (0);
}

static int	change_to_oldpwd(char ***env)
{
	char		*path;
	struct stat	file_stat;

	path = ft_getenv(*env, "OLDPWD");
	if (!path)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
		return (1);
	}
	if (check_dir_x(&file_stat, path))
	{
		if (chdir(path) == -1)
		{
			print_cd_error(path, 3);
			return (1);
		}
		ft_putstr_fd(path, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	else
	{
		check_path(path);
		return (1);
	}
	return (0);
}

static int	change_to_path(char *path)
{
	struct stat	file_stat;

	if (check_dir_x(&file_stat, path))
	{
		if (chdir(path) == -1)
		{
			print_cd_error(path, 3);
			return (1);
		}
	}
	else
	{
		if (access(path, F_OK) == -1)
			print_cd_error(path, 1);
		else if (S_ISDIR(file_stat.st_mode))
			print_cd_error(path, 3);
		else
			print_cd_error(path, 2);
		return (1);
	}
	return (0);
}

int	handle_directory_change(char *path, char ***env)
{
	if (!path)
		return (change_to_home(env));
	else if (!ft_strcmp(path, "-"))
		return (change_to_oldpwd(env));
	else
		return (change_to_path(path));
}

int	validate_and_get_path(char **argv, char **path)
{
	int	arg_count;

	arg_count = count_arguments(argv);
	if (arg_count > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (EXIT_FAILURE);
	}
	if (arg_count == 2)
		*path = argv[1];
	else
		*path = NULL;
	return (0);
}
