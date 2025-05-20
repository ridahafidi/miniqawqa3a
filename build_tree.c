/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:50:58 by yel-qori          #+#    #+#             */
/*   Updated: 2025/05/19 19:36:27 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree *create_command()
{
	t_tree *node;
	
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
