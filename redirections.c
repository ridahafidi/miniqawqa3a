/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:39:56 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/25 17:05:13 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_heredoc_line(char *line, char *delimiter, int pipefd,
	char **env)
{
	char	**expanded;

	if (ft_strchr(delimiter, '\'') || ft_strchr(delimiter, '\"'))
	{
		write(pipefd, line, ft_strlen(line));
		write(pipefd, "\n", 1);
	}
	else
	{
		expanded = expand(&line, env, exit_status);
		write(pipefd, expanded[0], ft_strlen(expanded[0]));
		write(pipefd, "\n", 1);
		free_array(expanded);
	}
}

static int	heredoc_loop(char *tmp_delimiter, int pipefd, char **env,
	char *delimiter)
{
	char	*line[2];

	line[1] = NULL;
	while (1)
	{
		line[0] = readline("> ");
		if (!line[0])
		{
			write(STDOUT_FILENO, "\n", 1);
			break ;
		}
		if (!ft_strcmp(line[0], tmp_delimiter))
			break ;
		write_heredoc_line(line[0], delimiter, pipefd, env);
		free(line[0]);
	}
	free(line[0]);
	return (0);
}

int	handle_heredoc(char *delimiter, char **env)
{
	int		pipefd[2];
	char	*tmp_delimiter[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe failed");
		return (-1);
	}
	signal(SIGINT, heredoc_sigint_handler);
	tmp_delimiter[1] = NULL;
	tmp_delimiter[0] = remove_quotes_from_string(delimiter, 1);
	heredoc_loop(tmp_delimiter[0], pipefd[1], env, delimiter);
	close(pipefd[1]);
	signal(SIGINT, sigint_handler);
	free(tmp_delimiter[0]);
	return (pipefd[0]);
}

void	append(t_tree *root, int *in, int *out)
{
	if (root->type == APPEND && root->file_name)
	{
		if (*out != STDOUT_FILENO)
			close(*out);
		*out = open(root->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (*out == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(root->file_name, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			exit_status = 1;
		}
	}
}

static void	handle_less_redirection(t_tree *root, int *in)
{
	if (*in != STDIN_FILENO)
		close(*in);
	*in = open(root->file_name, O_RDONLY);
	if (*in == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(root->file_name, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		exit_status = 1;
	}
}

static void	handle_greater_redirection(t_tree *root, int *out)
{
	if (*out != STDOUT_FILENO)
		close(*out);
	*out = open(root->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*out == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(root->file_name, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		exit_status = 1;
	}
}

void	less_and_greater(t_tree *root, int *in, int *out)
{
	if (root->type == LESS && root->file_name)
		handle_less_redirection(root, in);
	else if (root->type == GREATER && root->file_name)
		handle_greater_redirection(root, out);
}

static t_tree	*check_ambiguous_redirection(t_tree *root, t_tree *cmd)
{
	if (root->file_name && !root->file_name[0])
	{
		ft_putstr_fd("minishell : ambigious redirection\n", 2);
		exit_status = 1;
		return (NULL);
	}
	if (!cmd && exit_status == 1)
		return (NULL);
	return (cmd);
}

static t_tree	*handle_greater_less(t_tree *root, int *in, int *out)
{
	t_tree	*cmd;

	cmd = handle_redirections(root->left, in, out, NULL);
	cmd = check_ambiguous_redirection(root, cmd);
	if (!cmd)
		return (NULL);
	if ((root->type == GREATER || root->type == LESS) && root->file_name)
	{
		less_and_greater(root, in, out);
		if (*in == -1 || *out == -1)
			return (NULL);
		return (cmd);
	}
	return (NULL);
}

static int	handle_heredoc_redirection(t_tree *root, int *in, char **env)
{
	if (*in != STDIN_FILENO)
		close(*in);
	*in = handle_heredoc(root->file_name, env);
	if (*in == -1)
	{
		perror("heredoc failed");
		exit_status = 1;
		return (-1);
	}
	return (0);
}

static t_tree	*handle_append_heredoc(t_tree *root, int *in, int *out,
	char **env)
{
	t_tree	*cmd;

	cmd = handle_redirections(root->left, in, out, env);
	cmd = check_ambiguous_redirection(root, cmd);
	if (!cmd)
		return (NULL);
	if (root->type == APPEND && root->file_name)
	{
		append(root, in, out);
		if (*in == -1 || *out == -1)
			return (NULL);
		return (cmd);
	}
	else if (root->type == HEREDOC && root->file_name)
	{
		if (handle_heredoc_redirection(root, in, env) == -1)
			return (NULL);
	}
	return (cmd);
}

t_tree	*handle_redirections(t_tree *root, int *in, int *out, char **env)
{
	if (!root)
		return (NULL);
	if (root->type == COMMAND)
		return (root);
	if (root->type == LESS || root->type == GREATER)
		return (handle_greater_less(root, in, out));
	else if (root->type == APPEND || root->type == HEREDOC)
		return (handle_append_heredoc(root, in, out, env));
	return (handle_redirections(root->left, in, out, env));
}