/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 20:30:31 by rhafidi           #+#    #+#             */
/*   Updated: 2024/11/03 21:35:31 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	fill(char *dst, const char *src, size_t length)
{
	while (length > 0)
	{
		*dst = *src;
		src++;
		dst++;
		length--;
	}
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len;
	char	*str;

	len = ft_strlen(s1) + ft_strlen(s2);
	str = malloc(sizeof(char) * (len + 1));
	if (str == NULL)
		return (NULL);
	str[len] = '\0';
	fill(str, s1, ft_strlen(s1));
	fill(str + ft_strlen(s1), s2, ft_strlen(s2));
	return (str);
}
