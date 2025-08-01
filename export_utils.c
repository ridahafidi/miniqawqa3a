/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:38:56 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/31 20:39:47 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_equal_or_append_pos(char *str)
{
	int	i;
	int	equal_pos;

	i = 0;
	equal_pos = -1;
	while (str[i] && str[i] != '=')
	{
		if (str[i] == '+' && str[i + 1] == '=')
		{
			equal_pos = i;
			break ;
		}
		i++;
	}
	if (str[i] == '=' && equal_pos == -1)
		equal_pos = i;
	return (equal_pos);
}

static int	validate_identifier_chars(char *str, int equal_pos)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	i++;
	while (str[i] && (equal_pos == -1 || i < equal_pos))
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	is_valid_identifier(char *str)
{
	int	equal_pos;

	if (!str || !*str)
		return (0);
	equal_pos = find_equal_or_append_pos(str);
	return (validate_identifier_chars(str, equal_pos));
}

char	**copy_env(char **env)
{
	int		len;
	char	**copy;
	int		i;

	len = 0;
	i = 0;
	if (!env)
		return (NULL);
	while (env[len])
		len++;
	copy = malloc(sizeof(char *) * (len + 1));
	if (!copy)
		return (NULL);
	copy[len] = NULL;
	while (env[i])
	{
		copy[i] = ft_strdup(env[i]);
		i++;
	}
	return (copy);
}

void	ft_swap(char **export, int i, int j)
{
	char	*tmp;

	if (export)
	{
		tmp = export[i];
		export[i] = export[j];
		export[j] = tmp;
	}
}
