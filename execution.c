/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:52:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/01 21:35:05 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int get_exit_status(int exit_status)
{
    if (WIFEXITED(exit_status))
        return (WEXITSTATUS(exit_status));
    else if (WIFSIGNALED(exit_status))
        return (128 + WTERMSIG(exit_status)); 
    return (EXIT_FAILURE);
}

void redirecting(int in, int out)
{
    dup2(in, STDIN_FILENO);
    dup2(out, STDOUT_FILENO);
    if (in != STDIN_FILENO)
        close(in);
    if (out != STDOUT_FILENO)
        close(out);
}

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
	execute_command(root, data, NULL);
}

 void	handle_parent_process(int pid, int *exit_status)
{
	int status;
	
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

void	pipein(int *pfd)
{
	if (pipe(pfd) == -1)
	{
		perror("pipe failed");
		exit(1);
	}
}

void	close_wait(int *pfd, t_pid *pid, t_fd *fd, int *exit_status)
{
	int status;
	
	close(pfd[0]);
	close(pfd[1]);
	waitpid(pid->left_pid, NULL, 0);
	waitpid(pid->right_pid, &status, 0);
	*exit_status = get_exit_status(status);
	free(pid);
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

 void	handle_right_pipe(t_tree *root,t_data *data, int *pfd, int *exit_status)
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
		return;
	}
	pipein(pfd);
	data->pid->left_pid = fork();
	if (!data->pid->left_pid)
		handle_left_pipe(root, data, pfd, exit_status);
	data->pid->right_pid = fork();
	if (!data->pid->right_pid)
		handle_right_pipe(root, data, pfd, exit_status);
	close_wait(pfd, data->pid, data->fds, exit_status);
	data->pid = saved_pid;
}

 void	handle_redirection_execution(t_tree *cmd, t_data *data, int *exit_status)
{
	if (cmd)
		forker(cmd, data, exit_status);
	else
		forker(NULL, data, exit_status);
	if (data->fds->in != STDIN_FILENO)
		close(data->fds->in);
	if (data->fds->out != STDOUT_FILENO)
		close(data->fds->out);
}

 void	handle_redirection_error(t_fd *fd)
{
	if (fd->in != STDIN_FILENO)
		close(fd->in);
	if (fd->out != STDOUT_FILENO)
		close(fd->out);
}

void	execution(t_tree *root, t_data *data, int *exit_status)
{
	t_tree	*cmd;

	if (!root)
		return ;
	if (root->type == APPEND || root->type == GREATER || root->type == LESS
		|| root->type == HEREDOC)
	{
		cmd = handle_redirections(root, data, exit_status);
		if (*exit_status != 0)
		{
			handle_redirection_error(data->fds);
			return ;
		}
		handle_redirection_execution(cmd, data, exit_status);
		return ;
	}
	else if (root->type == COMMAND)
		forker(root, data, exit_status);
	else if (root->type == PIPE)
		handle_pipe(data, root, exit_status);
}

int	initialize(t_tree *root, t_data *data, int *exit_status)
{
	execution(root, data, exit_status);
	return (*exit_status);
}
