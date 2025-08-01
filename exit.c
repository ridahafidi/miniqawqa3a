/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:51:42 by yel-qori          #+#    #+#             */
/*   Updated: 2025/08/01 22:08:01 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_exit_arg(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if ((arg[i] < '0' || arg[i] > '9') && arg[i] != '+' && arg[i] != '-')
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(t_tree *root, t_data *data, int *exit_status)
{
	int		exit_code;
	char	**argv;

	argv = root->command;
	clear_history();
	if (!argv[1])
		clean_exit(root, data, *exit_status);
	if (!validate_exit_arg(argv[1]))
	{
		exit_error(argv);
		free_mem(root, data->fds, data);
		exit(2);
	}
	if (argv[1] && argv[2])
		return (many_args());
	exit_code = ret_ex_code(argv);
	free_mem(root, data->fds, data);
	exit(exit_code & 0xFF);
}
