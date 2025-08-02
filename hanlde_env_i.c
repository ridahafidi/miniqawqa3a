/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hanlde_env_i.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 15:21:47 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 15:22:12 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**handle_env_i(void)
{
	char	**my_env;
	char	cwd[4096];

	my_env = malloc(sizeof(char *) * 4);
	if (!my_env)
		return (NULL);
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("getcwd failed ");
		free(my_env);
		return (NULL);
	}
	my_env[0] = ft_strjoin("PWD=", cwd);
	my_env[1] = ft_strdup("SHLVL=1");
	my_env[2] = ft_strdup("_=/usr/bin/env");
	my_env[3] = NULL;
	return (my_env);
}
