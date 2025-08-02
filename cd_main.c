/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:36:59 by yel-qori          #+#    #+#             */
/*   Updated: 2025/08/02 20:17:33 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_current_pwd_from_env(int *should_free, char ***env)
{
	char	*current_pwd;
	char	*tmp;

	tmp = ft_getenv(*env, "PWD");
	*should_free = 0;
	if (tmp)
	{
		current_pwd = ft_strdup(tmp);
		if (!current_pwd)
			return (NULL);
		*should_free = 1;
		return (current_pwd);
	}
	current_pwd = malloc(4096);
	*should_free = 1;
	if (!current_pwd || getcwd(current_pwd, 4096) == NULL)
	{
		if (current_pwd)
			free(current_pwd);
		return (NULL);
	}
	return (current_pwd);
}

static int	handle_cd_operation(char *path, char ***env, char *current_pwd)
{
	char	new_cwd[4096];

	if (handle_directory_change(path, env) != 0)
		return (1);
	if (getcwd(new_cwd, sizeof(new_cwd)) == NULL)
	{
		ft_putstr_fd("minishell: cd: error retrieving current directory\n",
			STDERR_FILENO);
		return (1);
	}
	update_env(env, new_cwd, current_pwd);
	return (0);
}

int	ft_cd(char **argv, char ***env)
{
	char	*path;
	char	*current_pwd;
	int		should_free_pwd;
	int		result;

	if (validate_and_get_path(argv, &path) != 0)
		return (EXIT_FAILURE);
	current_pwd = get_current_pwd_from_env(&should_free_pwd, env);
	if (!current_pwd)
	{
		ft_putstr_fd("minishell: cd: error retrieving current directory\n",
			STDERR_FILENO);
		return (1);
	}
	result = handle_cd_operation(path, env, current_pwd);
	if (should_free_pwd)
		free(current_pwd);
	return (result);
}
