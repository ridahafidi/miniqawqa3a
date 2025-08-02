/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:27:23 by yel-qori          #+#    #+#             */
/*   Updated: 2025/08/02 15:26:33 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_signals_and_check(int *exit_status)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	check_received_signal(exit_status);
}

void	handle_null_input(char ***env, char ***exported, int exit_status)
{
	free_array(env[0]);
	free_array(exported[0]);
	ctrl_d_handle(0, exit_status);
}

void	shell_loop(char ***env, char ***exported, int *exit_status)
{
	char	*input;
	char	exit;
	int		ret;

	exit = 0;
	while (!exit)
	{
		setup_signals_and_check(exit_status);
		input = readline("minishell> ");
		check_received_signal(exit_status);
		if (input == NULL)
			handle_null_input(env, exported, *exit_status);
		ret = process_input_line(input, env, exported, exit_status);
		exit = ret;
		if (input)
			free(input);
		input = NULL;
	}
	clear_history();
}

int	main(int ac, char **av, char **env)
{
	char	**my_env;
	int		exit_status;
	char	**exported;

	(void)av;
	(void)ac;
	exit_status = 0;
	g_signum = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	my_env = copy_env(env);
	update_shlvl(&my_env);
	if (!my_env[0])
		my_env = handle_env_i();
	exported = copy_env(env);
	shell_loop(&my_env, &exported, &exit_status);
	free_array(my_env);
	free_array(exported);
	return (exit_status);
}
