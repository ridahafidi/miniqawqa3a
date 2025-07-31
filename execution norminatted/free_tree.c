/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:20:39 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/25 17:55:18 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_tree(t_tree **root)
{
    int i = 0;
    if (!*root || !root)
        return ;
        free_tree(&(*root)->left);
        free_tree(&(*root)->right);
        if ((*root)->type != HEREDOC)
        {
            if ((*root)->command)
            {
                while ((*root)->command[i])
                {
                    if ((*root)->command[i])
                    free((*root)->command[i]);
                    i++;
                }    
            }
        }
    if((*root)->command)
    free((*root)->command);
    if ((*root)->file_name)
    free((*root)->file_name);
    free((*root));
}
