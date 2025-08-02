/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 18:59:57 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 19:22:03 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_signum = 0;

void	ctrl_d_handle(int sig, int exit_status)
{
	(void)sig;
	printf("exit\n");
	clear_history();
	exit(exit_status);
}

void	sigint_handler(int sig)
{
	(void)sig;
	g_signum = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_signum = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	child_sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	exit(EXIT_SIGINT_CODE);
}

int	check_received_signal(int *exit_status)
{
	if (g_signum == SIGINT)
	{
		*exit_status = EXIT_SIGINT_CODE;
		g_signum = 0;
		return (1);
	}
	return (0);
}
