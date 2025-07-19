/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:20:39 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/19 15:14:54 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_tree(t_tree **root)
{
    int i = 0;
    // printf("i am freeeeing that shit\n");
    if (!*root || !root)
        return ;
        
        free_tree(&(*root)->left);
        // free((*root)->command);
        free_tree(&(*root)->right);
        
        // printf("%d\n", (*root)->type);
        if ((*root)->type != HEREDOC)
        {
            // printf("here\n");
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
    // // printf("%d\n", (*root)->type);
    if((*root)->command)
    free((*root)->command);
    if ((*root)->file_name)
    free((*root)->file_name);
    free((*root));
}