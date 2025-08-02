/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 17:56:06 by rhafidi           #+#    #+#             */
/*   Updated: 2024/11/07 10:12:24 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*tmp_lst;

	tmp_lst = (*lst);
	if (!(*lst))
		*lst = new;
	else
	{
		tmp_lst = ft_lstlast(tmp_lst);
		tmp_lst->next = new;
	}
}
