/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 18:59:57 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/19 21:27:23 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int exit_status;

void	ctrl_d_handle(int sig)
{
	(void)sig;
	printf("exit\n");
	clear_history();
	exit(EXIT_SUCCESS);
}

void	sigint_handler(int sig)
{
	(void)sig;
	exit_status = EXIT_SIGINT_CODE;  // 130 for Ctrl-C
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	exit_status = EXIT_SIGINT_CODE;  // 130 for Ctrl-C
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	child_sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	// clear_history();
	exit(EXIT_SIGINT_CODE);  // 130 for Ctrl-C
}