/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:33:01 by rhafidi           #+#    #+#             */
/*   Updated: 2024/11/09 12:26:23 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	if (!(*lst))
		return ;
	if ((*lst) && (*lst)->next)
		ft_lstclear(&((*lst)->next), del);
	(del)((*lst)->content);
	(*lst)->next = NULL;
	free(*lst);
	(*lst) = NULL;
}
