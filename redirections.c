/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:39:56 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 19:21:30 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_less_redirection(t_tree *root, int *in, int *exit_status)
{
	if (*in != STDIN_FILENO)
		close(*in);
	*in = open(root->file_name, O_RDONLY);
	if (*in == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(root->file_name, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		if (exit_status)
			*exit_status = 1;
	}
}

void	handle_greater_redirection(t_tree *root, int *out, int *exit_status)
{
	if (*out != STDOUT_FILENO)
		close(*out);
	*out = open(root->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*out == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(root->file_name, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		if (exit_status)
			*exit_status = 1;
	}
}

void	less_and_greater(t_tree *root, int *in, int *out, int *exit_status)
{
	if (root->type == LESS && root->file_name)
		handle_less_redirection(root, in, exit_status);
	else if (root->type == GREATER && root->file_name)
		handle_greater_redirection(root, out, exit_status);
}

t_tree	*handle_greater_less(t_tree *root, t_data *data, int *exit_status)
{
	t_tree	*cmd;
	int		*in;
	int		*out;

	in = &data->fds->in;
	out = &data->fds->out;
	cmd = handle_redirections(root->left, data, exit_status);
	cmd = check_ambiguous_redirection(root, cmd, exit_status);
	if (!cmd)
		return (NULL);
	if ((root->type == GREATER || root->type == LESS) && root->file_name)
	{
		less_and_greater(root, in, out, exit_status);
		if (*in == -1 || *out == -1)
			return (NULL);
		return (cmd);
	}
	return (NULL);
}

t_tree	*handle_redirections(t_tree *root, t_data *data, int *exit_status)
{
	if (!root)
		return (NULL);
	if (root->type == COMMAND)
		return (root);
	if (root->type == LESS || root->type == GREATER)
		return (handle_greater_less(root, data, exit_status));
	else if (root->type == APPEND || root->type == HEREDOC)
		return (handle_append_heredoc(root, data, exit_status));
	return (handle_redirections(root->left, data, exit_status));
}
