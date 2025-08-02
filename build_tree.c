/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:50:58 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/25 15:50:50 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*create_command(void)
{
	t_tree	*node;

	node = malloc(sizeof(t_tree));
	if (!node)
		return (NULL);
	node->command = NULL;
	node->file_name = NULL;
	node->left = NULL;
	node->right = NULL;
	node->type = 0;
	return (node);
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}
