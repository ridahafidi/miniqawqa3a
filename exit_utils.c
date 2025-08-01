/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:50:21 by yel-qori          #+#    #+#             */
/*   Updated: 2025/08/01 22:05:34 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_mem(t_tree *root, t_fd *fd, t_data *data)
{
	free_tree(&root);
	free(fd);
	free_array(data->env[0]);
	free_array(data->exported[0]);
	if (data)
		free(data);
}

void	exit_error(char **argv)
{
	ft_putstr_fd("exit\n", STDERR_FILENO);
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(argv[1], STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
}

int	many_args(void)
{
	ft_putstr_fd("exit\n", STDERR_FILENO);
	ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
	return (EXIT_FAILURE);
}

int	ret_ex_code(char **argv)
{
	int	exit_code;

	if (argv[1])
		exit_code = ft_atoi(argv[1]);
	else
		exit_code = 0;
	return (exit_code);
}

void	clean_exit(t_tree *root, t_data *data, int exit_status)
{
	free_mem(root, data->fds, data);
	exit(exit_status);
}
