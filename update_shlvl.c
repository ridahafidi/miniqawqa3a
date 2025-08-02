/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_shlvl.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 15:20:12 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 15:20:52 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_shlvl_value(char ***env, int i)
{
	char	*new_shlvl;
	char	*val;
	int		new_val;

	new_val = ft_atoi(&env[0][i][ft_strlen("SHLVL") + 1]);
	new_val++;
	val = ft_itoa(new_val);
	new_shlvl = ft_strjoin("SHLVL=", val);
	(free(val), free(env[0][i]));
	env[0][i] = new_shlvl;
}

void	update_shlvl(char ***env)
{
	int	i;

	i = 0;
	if (!env[0][i])
		return ;
	while (env[0][i])
	{
		if (!strncmp("SHLVL", env[0][i], ft_strlen("SHLVL") - 1)
			&& env[0][i][ft_strlen("SHLVL")] == '=')
		{
			update_shlvl_value(env, i);
			return ;
		}
		i++;
	}
}
