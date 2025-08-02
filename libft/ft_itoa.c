/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 20:32:07 by rhafidi           #+#    #+#             */
/*   Updated: 2024/11/09 13:25:53 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	countlen(int n)
{
	int	len;

	len = 0;
	if (n == -2147483648)
	{
		n = n / 10;
		len++;
	}
	if (n < 0)
	{
		n = -n;
		len++;
	}
	if (n == 0)
		len++;
	else
	{
		while (n)
		{
			n = n / 10;
			len++;
		}
	}
	return (len);
}

static void	fill(char *str, int n, int len, int negative)
{
	str[len] = '\0';
	len--;
	if (n == -2147483648)
	{
		str[len] = '8';
		len--;
		n = n / 10;
	}
	if (negative == 1)
	{
		str[0] = '-';
		n = -n;
	}
	if (n == 0)
		str[len] = '0';
	if (n > 0)
	{
		while (n)
		{
			str[len] = (n % 10) + '0';
			n = n / 10;
			len--;
		}
	}
}

char	*ft_itoa(int n)
{
	int		len;
	int		negative;
	char	*str;

	len = countlen(n);
	negative = 0;
	if (n < 0)
		negative = 1;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	fill(str, n, len, negative);
	return (str);
}
