/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 15:38:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 20:23:41 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_wait(int *pfd, t_pid *pid, int *exit_status)
{
	int	status;

	close(pfd[0]);
	close(pfd[1]);
	waitpid(pid->left_pid, NULL, 0);
	waitpid(pid->right_pid, &status, 0);
	*exit_status = get_exit_status(status);
	free(pid);
}

void	pipein(int *pfd)
{
	if (pipe(pfd) == -1)
	{
		perror("pipe failed");
		exit(1);
	}
}

void	handle_left_pipe(t_tree *root, t_data *data, int *pfd, int *exit_status)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(pfd[0]);
	data->fds->out = pfd[1];
	execution(root->left, data, exit_status);
	free_array(data->exported[0]);
	free_array(data->env[0]);
	clear_history();
	free_tree(&root);
	if (data->pid)
		free(data->pid);
	if (data->fds)
		free(data->fds);
	if (data)
		free(data);
	exit(*exit_status);
}

void	handle_right_pipe(t_tree *root, t_data *data, int *pfd,
		int *exit_status)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(pfd[1]);
	data->fds->in = pfd[0];
	execution(root->right, data, exit_status);
	free_array(data->exported[0]);
	free_array(data->env[0]);
	clear_history();
	free_tree(&root);
	if (data->pid)
		free(data->pid);
	if (data->fds)
		free(data->fds);
	if (data)
		free(data);
	exit(*exit_status);
}

void	handle_pipe(t_data *data, t_tree *root, int *exit_status)
{
	int		pfd[2];
	t_pid	*saved_pid;

	saved_pid = data->pid;
	data->pid = malloc(sizeof(t_pid));
	if (!data->pid)
	{
		data->pid = saved_pid;
		if (data->fds)
			free(data->fds);
		if (data)
			free(data);
		return ;
	}
	pipein(pfd);
	data->pid->left_pid = fork();
	if (!data->pid->left_pid)
		handle_left_pipe(root, data, pfd, exit_status);
	data->pid->right_pid = fork();
	if (!data->pid->right_pid)
		handle_right_pipe(root, data, pfd, exit_status);
	close_wait(pfd, data->pid, exit_status);
	data->pid = saved_pid;
}
