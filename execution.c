/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:52:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/25 16:30:59 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int get_exit_status()
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

static void	handle_empty_command(t_fd *fd)
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

static void	handle_builtin_command(t_tree *root, t_fd *fd, char ***env,
	char ***exported)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	redirecting(fd->in, fd->out);
	if (fd->in == -1 || fd->out == -1)
	{
		exit_status = 1;
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return ;
	}
	exit_status = handle_builtins(root, fd, env, exported, exit_status);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

static void	handle_child_process(t_tree *root, t_fd *fd, char **env,
	char **exported)
{
	signal(SIGINT, child_sigint_handler);
	signal(SIGQUIT, SIG_DFL);
	redirecting(fd->in, fd->out);
	if (fd->in == -1 || fd->out == -1)
	{
		free_array(env);
		free_array(exported);
		clear_history();
		free_tree(&root);
		exit(EXIT_FAILURE);
	}
	execute_command(root, fd, env, exported);
}

static void	handle_parent_process(int pid)
{
	waitpid(pid, &exit_status, 0);
	if (WIFSIGNALED(exit_status))
	{
		if (WTERMSIG(exit_status) == SIGQUIT)
		{
			ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
			exit_status = 131;
		}
		else if (WTERMSIG(exit_status) == SIGINT)
		{
			ft_putstr_fd("\n", STDERR_FILENO);
			exit_status = 130;
		}
	}
	else if (WIFEXITED(exit_status))
		exit_status = WEXITSTATUS(exit_status);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	forker(t_tree *root, t_fd *fd, char ***env, char ***exported)
{
	int	pid;

	if (!root->command || !root->command[0])
	{
		handle_empty_command(fd);
		return ;
	}
	if (!is_builtin(root->command[0]))
	{
		handle_builtin_command(root, fd, env, exported);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed :");
		exit(1);
	}
	if (!pid)
		handle_child_process(root, fd, *env, *exported);
	else
		handle_parent_process(pid);
}

void	pipein(int *pfd)
{
	if (pipe(pfd) == -1)
	{
		perror("pipe failed");
		exit(1);
	}
}

void	close_wait(int *pfd, t_pid *pid, t_fd *fd)
{
	close(pfd[0]);
	close(pfd[1]);
	waitpid(pid->left_pid, NULL, 0);
	waitpid(pid->right_pid, &exit_status, 0);
	exit_status = get_exit_status();
	free(pid);
}

void	free_exit(t_pid *pid, t_fd *fd)
{
	free(pid);
	free(fd);
	exit(exit_status);
}

static void	handle_left_pipe(t_tree *root, t_fd *fd, char ***env,
	char ***exported, int *pfd, t_pid *pid)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(pfd[0]);
	fd->out = pfd[1];
	execution(root->left, fd, env, exported);
	free_array(*exported);
	free_array(*env);
	clear_history();
	free_tree(&root);
	free_exit(pid, fd);
}

static void	handle_right_pipe(t_tree *root, t_fd *fd, char ***env,
	char ***exported, int *pfd, t_pid *pid)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(pfd[1]);
	fd->in = pfd[0];
	execution(root->right, fd, env, exported);
	free_array(*exported);
	free_array(*env);
	clear_history();
	free_tree(&root);
	free_exit(pid, fd);
}

void	handle_pipe(t_fd *fd, char ***env, char ***exported, t_tree *root)
{
	int		pfd[2];
	t_pid	*pid;

	pid = malloc(sizeof(t_pid));
	pipein(pfd);
	pid->left_pid = fork();
	if (!pid->left_pid)
		handle_left_pipe(root, fd, env, exported, pfd, pid);
	pid->right_pid = fork();
	if (!pid->right_pid)
		handle_right_pipe(root, fd, env, exported, pfd, pid);
	close_wait(pfd, pid, fd);
}

static void	handle_redirection_execution(t_tree *cmd, t_fd *fd, char ***env,
	char ***exported)
{
	if (cmd)
		forker(cmd, fd, env, exported);
	else
		forker(NULL, fd, env, exported);
	if (fd->in != STDIN_FILENO)
		close(fd->in);
	if (fd->out != STDOUT_FILENO)
		close(fd->out);
}

static void	handle_redirection_error(t_fd *fd)
{
	if (fd->in != STDIN_FILENO)
		close(fd->in);
	if (fd->out != STDOUT_FILENO)
		close(fd->out);
}

void	execution(t_tree *root, t_fd *fd, char ***env, char ***exported)
{
	t_tree	*cmd;

	if (!root)
		return ;
	if (root->type == APPEND || root->type == GREATER || root->type == LESS
		|| root->type == HEREDOC)
	{
		cmd = handle_redirections(root, &fd->in, &fd->out, env[0]);
		if (exit_status != 0)
		{
			handle_redirection_error(fd);
			return ;
		}
		handle_redirection_execution(cmd, fd, env, exported);
		return ;
	}
	else if (root->type == COMMAND)
		forker(root, fd, env, exported);
	else if (root->type == PIPE)
		handle_pipe(fd, env, exported, root);
}

int	initialize(t_tree *root, t_fd *fd, char ***env, char ***exported)
{
	execution(root, fd, env, exported);
	return (exit_status);
}
