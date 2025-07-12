/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:24:42 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/12 20:39:40 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int is_pipe(char *token)
// {
//     if (!token)
//         return (0);
//     return (ft_strcmp(token, "|") == 0);
// }

void    syntaxe_error()
{
    ft_putstr_fd("bash: syntax error near unexpected token", 2);
    exit_status = 2;
}
