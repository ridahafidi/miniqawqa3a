/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:24:42 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/31 18:38:45 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	syntaxe_error(void)
{
	ft_putstr_fd("bash: syntax error near unexpected token", 2);
}

int	is_redirection(char *token)
{
	return (!ft_strcmp(token, "<") || !ft_strcmp(token, ">")
		|| !ft_strcmp(token, ">>") || !ft_strcmp(token, "<<"));
}

int	is_pipe(char *token)
{
	return (!ft_strcmp(token, "|"));
}

int	check_first_token_pipe(char **tokens)
{
	if (is_pipe(tokens[0]))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (1);
	}
	return (0);
}

void	print_syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (!token)
		ft_putstr_fd("newline", 2);
	else
		ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
}
