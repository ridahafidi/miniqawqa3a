/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 15:35:55 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 15:36:14 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirection_execution(t_tree *cmd, t_data *data,
		int *exit_status)
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
