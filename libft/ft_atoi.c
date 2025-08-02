/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 20:29:44 by rhafidi           #+#    #+#             */
/*   Updated: 2024/11/03 21:30:47 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	char	*temp_nptr;
	int		sign;
	int		result;

	result = 0;
	sign = 1;
	temp_nptr = (char *)nptr;
	while (*temp_nptr == ' ' || (*temp_nptr >= 9 && *temp_nptr <= 13))
		temp_nptr++;
	if (*temp_nptr == '-')
	{
		sign = -1;
		temp_nptr++;
	}
	else if (*temp_nptr == '+')
		temp_nptr++;
	while (ft_isdigit(*temp_nptr))
	{
		result = (result * 10) + (*temp_nptr - '0');
		temp_nptr++;
	}
	return (result * sign);
}
