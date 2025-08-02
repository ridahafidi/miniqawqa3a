/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_err.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 15:23:36 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 15:23:58 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_parsing_error(char **tokens, t_data *data, int *exit_status)
{
	if (tokens)
	{
		free_array(tokens);
		*exit_status = 2;
	}
	if (data->fds)
		free(data->fds);
	if (data)
		free(data);
	return (0);
}

int	handle_tokenization_error(t_fd *fds, t_data *data, int *exit_status)
{
	free(fds);
	free(data);
	*exit_status = 2;
	return (0);
}
