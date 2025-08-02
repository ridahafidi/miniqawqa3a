/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 17:45:12 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 20:15:51 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*check_ambiguous_redirection(t_tree *root, t_tree *cmd,
		int *exit_status)
{
	if (root->file_name && !root->file_name[0])
	{
		ft_putstr_fd("minishell : ambigious redirection\n", 2);
		if (exit_status)
			*exit_status = 1;
		return (NULL);
	}
	if (!cmd && exit_status && *exit_status == 1)
		return (NULL);
	return (cmd);
}

void	append(t_tree *root, int *out, int *exit_status)
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
			if (exit_status)
				*exit_status = 1;
		}
	}
}

int	handle_heredoc_redirection(t_tree *root, int *in, char **env,
		int *exit_status)
{
	if (*in != STDIN_FILENO)
		close(*in);
	*in = handle_heredoc(root->file_name, env);
	if (*in == -1)
	{
		perror("heredoc failed");
		if (exit_status)
			*exit_status = 1;
		return (-1);
	}
	return (0);
}

t_tree	*handle_append_heredoc(t_tree *root, t_data *data, int *exit_status)
{
	t_tree	*cmd;

	cmd = handle_redirections(root->left, data, exit_status);
	cmd = check_ambiguous_redirection(root, cmd, exit_status);
	if (!cmd)
		return (NULL);
	if (root->type == APPEND && root->file_name)
	{
		append(root, &data->fds->out, exit_status);
		if (data->fds->in == -1 || data->fds->out == -1)
			return (NULL);
		return (cmd);
	}
	else if (root->type == HEREDOC && root->file_name)
	{
		if (handle_heredoc_redirection(root, &data->fds->in, data->env[0],
				exit_status) == -1)
			return (NULL);
	}
	return (cmd);
}
