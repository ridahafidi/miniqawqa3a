/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 15:34:43 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 19:20:36 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
