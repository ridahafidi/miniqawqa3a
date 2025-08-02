/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 20:30:52 by rhafidi           #+#    #+#             */
/*   Updated: 2024/11/09 12:29:39 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	isset(char s1, char const *set)
{
	while (*set)
	{
		if (*set == s1)
			return (1);
		set++;
	}
	return (0);
}

static void	fill(char *dst, char const *src, size_t len)
{
	while (len)
	{
		*dst = *src;
		src++;
		dst++;
		len--;
	}
	*dst = '\0';
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;
	int		len;
	char	*str;

	if (!s1 || !set)
		return (ft_strdup(""));
	len = 0;
	i = 0;
	j = ft_strlen(s1) - 1;
	while (isset(s1[i], set))
		i++;
	while (j >= i && isset(s1[j], set))
		j--;
	len = j - i + 1;
	if (len <= 0)
		return (ft_strdup(""));
	str = malloc(sizeof(char) * (len + 1));
	if (str == NULL)
		return (NULL);
	fill(str, s1 + i, len);
	return (str);
}
