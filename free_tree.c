/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:20:39 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/18 22:42:38 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_tree(t_tree **root)
{
    int i = 0;
    printf("i am freeeeing that shit\n");
    if (!*root)
        return ;
    free_tree(&(*root)->left);
    // free((*root)->command);
    free_tree(&(*root)->right);
    
    while ((*root)->command[i])
    {
        // printf("freeing\n");
        free((*root)->command[i]);
        i++;
    }
    if((*root)->command)
        free((*root)->command);
    if ((*root)->file_name)
        free((*root)->file_name);
    free((*root));
}