/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 18:59:57 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/31 18:45:05 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Global variable for signal handling - ONLY stores signal number
int g_signum = 0;

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
	g_signum = SIGINT;  // ONLY store the signal number, nothing else
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_signum = SIGINT;  // ONLY store the signal number, nothing else
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	child_sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	exit(EXIT_SIGINT_CODE);  // 130 for Ctrl-C
}

// Function to check and handle received signals
// Returns 1 if a signal was handled, 0 otherwise
int check_received_signal(int *exit_status)
{
    if (g_signum == SIGINT)
    {
        // Set exit status to 130 for SIGINT
        *exit_status = EXIT_SIGINT_CODE;
        
        // Reset the global signal variable after processing
        g_signum = 0;
        return 1;
    }
    
    return 0;
}