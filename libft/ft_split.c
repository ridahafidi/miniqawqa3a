/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 20:20:18 by rhafidi           #+#    #+#             */
/*   Updated: 2024/11/09 13:58:07 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	countwords(const char *s, char c)
{
	int		inside_word;
	size_t	found;

	found = 0;
	while (*s)
	{
		inside_word = 0;
		while (*s == c && *s)
			s++;
		while (*s != c && *s)
		{
			if (!inside_word)
			{
				found++;
				inside_word = 1;
			}
			s++;
		}
	}
	return (found);
}

static int	safemalloc(char **str, int i, size_t len)
{
	int	j;

	j = 0;
	str[i] = malloc(sizeof(char) * (len + 1));
	if (!str[i])
	{
		while (j <= i)
		{
			free(str[j]);
			j++;
		}
		return (1);
	}
	return (0);
}

static void	copy(char *dst, const char *src, size_t len)
{
	while (len)
	{
		*dst = *src;
		dst++;
		src++;
		len--;
	}
	*dst = '\0';
}

static int	fill(char **str, const char *s, char c)
{
	int		i;
	size_t	len;

	i = 0;
	while (*s)
	{
		len = 0;
		while (*s == c && *s)
			s++;
		while (*s != c && *s)
		{
			s++;
			len++;
		}
		if (len > 0)
		{
			if (!safemalloc(str, i, len))
				copy(str[i], s - len, len);
			else
				return (1);
			i++;
		}
	}
	return (0);
}

char	**ft_split(const char *s, char c)
{
	size_t	words;
	char	**str;
	size_t	i;

	if (s == NULL)
		return (NULL);
	i = 0;
	words = countwords(s, c);
	str = malloc(sizeof(char *) * (words + 1));
	if (str == NULL)
		return (NULL);
	str[words] = NULL;
	if (!fill(str, s, c))
		return (str);
	while (i <= words)
		free(str[i++]);
	free(str);
	return (NULL);
}
