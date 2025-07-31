/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:41:50 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/25 15:41:54 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_heredoc_recursion(t_tree *ast)
{
	if (ast->left)
		strip_quotes_from_ast(ast->left);
}

void	handle_normal_recursion(t_tree *ast)
{
	if (ast->left)
		strip_quotes_from_ast(ast->left);
	if (ast->right)
		strip_quotes_from_ast(ast->right);
}

void	strip_quotes_from_ast(t_tree *ast)
{
	if (!ast)
		return ;
	if (ast->type == COMMAND && ast->command)
	{
		strip_quotes_from_command(ast);
	}
	if (ast->type == HEREDOC)
	{
		handle_heredoc_recursion(ast);
		return ;
	}
	handle_normal_recursion(ast);
}

t_tree	*create_command_node(char **cmd_args)
{
	t_tree	*cmd_node;

	cmd_node = create_command();
	if (!cmd_node)
		return (NULL);
	cmd_node->type = COMMAND;
	cmd_node->command = cmd_args;
	return (cmd_node);
}

int	has_file_argument(int i, int end)
{
	if (i + 1 <= end)
		return (1);
	return (0);
}
