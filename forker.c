/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:52:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 20:19:04 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_empty_command(t_fd *fd)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	redirecting(fd->in, fd->out);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

void	handle_builtin_command(t_tree *root, t_data *data, int *exit_status)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	redirecting(data->fds->in, data->fds->out);
	if (data->fds->in == -1 || data->fds->out == -1)
	{
		*exit_status = 1;
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return ;
	}
	*exit_status = handle_builtins(root, data, exit_status);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

void	handle_child_process(t_tree *root, t_data *data)
{
	signal(SIGINT, child_sigint_handler);
	signal(SIGQUIT, SIG_DFL);
	redirecting(data->fds->in, data->fds->out);
	if (data->fds->in == -1 || data->fds->out == -1)
	{
		free_array(data->env[0]);
		free_array(data->exported[0]);
		clear_history();
		free_tree(&root);
		exit(EXIT_FAILURE);
	}
	execute_command(root, data);
}

void	handle_parent_process(int pid, int *exit_status)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
		{
			ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
			*exit_status = 131;
		}
		else if (WTERMSIG(status) == SIGINT)
		{
			ft_putstr_fd("\n", STDERR_FILENO);
			*exit_status = 130;
		}
	}
	if (WIFEXITED(status))
		*exit_status = WEXITSTATUS(status);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	forker(t_tree *root, t_data *data, int *exit_status)
{
	int	pid;

	if (!root->command || !root->command[0])
	{
		handle_empty_command(data->fds);
		return ;
	}
	if (!is_builtin(root->command[0]))
	{
		handle_builtin_command(root, data, exit_status);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed :");
		exit(1);
	}
	if (!pid)
		handle_child_process(root, data);
	else
		handle_parent_process(pid, exit_status);
}
