/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:20:39 by rhafidi           #+#    #+#             */
/*   Updated: 2025/05/19 19:36:21 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_tree(t_tree **root)
{
    if (!*root)
        return ;
    free_tree(&(*root)->left);
    free_tree(&(*root)->right);
    free((*root)->command);
    free((*root));
}