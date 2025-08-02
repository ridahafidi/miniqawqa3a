/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:32:02 by yel-qori          #+#    #+#             */
/*   Updated: 2025/08/02 20:18:34 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_args(char **argv, int i)
{
	while (argv[i])
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		i++;
		if (argv[i])
			write(STDOUT_FILENO, " ", 1);
	}
}

int	no_args(void)
{
	write(STDOUT_FILENO, "\n", 1);
	return (0);
}

int	ft_echo(char **argv)
{
	int	i;
	int	n_flag;
	int	j;

	i = 1;
	n_flag = 0;
	if (!argv[i])
		return (no_args());
	while (argv[i] && argv[i][0] == '-')
	{
		j = 1;
		while (argv[i][j] == 'n')
			j++;
		if (argv[i][j] != '\0')
			break ;
		if (j == 1)
			break ;
		n_flag = 1;
		i++;
	}
	print_args(argv, i);
	if (n_flag == 0)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
