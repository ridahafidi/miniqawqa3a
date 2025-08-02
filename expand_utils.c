/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 17:00:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 19:19:32 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_before_dollar(char *str, int dollar_index)
{
	int		i;
	char	*res;

	i = 0;
	res = malloc(sizeof(char) * dollar_index);
	if (!res || !dollar_index)
		return (NULL);
	while (i <= dollar_index)
	{
		res[i] = str[i];
		i++;
	}
	res[dollar_index] = '\0';
	return (res);
}

void	process_dollar_sequence(char *str, char *result, int *i, int *ri)
{
	int	dollar_count;

	dollar_count = 0;
	while (str[*i + dollar_count] == '$')
		dollar_count++;
	while (dollar_count--)
		result[(*ri)++] = '$';
	*i += dollar_count;
}

char	*expand_dollars(char *str)
{
	char	*result;
	int		i;
	int		ri;
	size_t	len;

	i = 0;
	ri = 0;
	len = ft_strlen(str);
	result = malloc(len * 2 + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '$')
			process_dollar_sequence(str, result, &i, &ri);
		else
			result[ri++] = str[i++];
	}
	result[ri] = '\0';
	return (result);
}

int	check_for_dollar(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

int	find_var_end(char *str)
{
	int	i;

	i = 1;
	if (!str[i])
		return (1);
	if (str[i] == '?' || str[i] == '$')
		return (2);
	if (str[i] == '{')
	{
		i++;
		while (str[i] && str[i] != '}')
			i++;
		if (str[i] == '}')
			return (i + 1);
		return (1);
	}
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}
