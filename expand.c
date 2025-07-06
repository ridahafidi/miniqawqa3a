/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:32:41 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/06 18:58:49 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*get_var_name(char *str)
{
    int     i;
    int     len;
    char    *var_name;

    i = 1;
    len = 0;

    // Handle $? case
    if (str[i] == '?')
        return ft_strdup("?");
    
    // Handle $$ case
    if (str[i] == '$')
        return ft_strdup("$");

    // Handle ${VAR} case
    if (str[i] == '{')
    {
        i++;
        while (str[i + len] && str[i + len] != '}')
            len++;
        if (str[i + len] != '}')
            return NULL;
        var_name = malloc(sizeof(char) * (len + 1));
        if (!var_name)
            return NULL;
        len = 0;
        while (str[i] && str[i] != '}')
            var_name[len++] = str[i++];
        var_name[len] = '\0';
        return var_name;
    }

    // Handle regular $VAR case
    while (str[i + len] && (ft_isalnum(str[i + len]) || str[i + len] == '_'))
        len++;
    
    if (len == 0)
        return NULL;
    
    var_name = malloc(sizeof(char) * (len + 1));
    if (!var_name)
        return NULL;
    
    i = 1;
    len = 0;
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
        var_name[len++] = str[i++];
    var_name[len] = '\0';
    
    return var_name;
}

char	*get_var_value(char *var_name, char **env, int status)
{
    int     i;
    char    *value;
    char    *status_str;
    pid_t   pid;

    if (!var_name)
        return ft_strdup("$");
    
    if (!ft_strcmp(var_name, "?"))
    {
        status_str = ft_itoa(status);
        return status_str;
    }

    if (!ft_strcmp(var_name, "$"))
    {
        pid = getpid();
        status_str = ft_itoa(pid);
        return status_str;
    }

    i = compare_var_env(var_name, env);
    if (i >= 0)
    {
        value = ft_substr(env[i], find_start(env[i]), ft_strlen(env[i]));
        return value;
    }
    return ft_strdup("");  // Return empty string for unset variables
}

int	find_var_end(char *str)
{
    int i;
    
    i = 1;
    
    if (!str[i])
        return 1;

    if (str[i] == '?' || str[i] == '$')
        return 2;

    if (str[i] == '{')
    {
        i++;
        while (str[i] && str[i] != '}')
            i++;
        return (str[i] == '}') ? i + 1 : 1;
    }
    
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
        i++;
    
    return i;
}

char *expand_string(char *str, char **env, int status)
{
    char    *result;
    char    *tmp;
    int     i;
    int     real_status;

    if (!str)
        return (NULL);
    
    result = ft_strdup("");
    if (!result)
        return (NULL);

    i = 0;
    while (str[i])
    {
        if (str[i] == '$')
        {
            // Handle $? - exit status
            if (str[i + 1] == '?')
            {
                if (WIFEXITED(status))
                    real_status = WEXITSTATUS(status);
                else
                    real_status = status;
                char *status_str = ft_itoa(real_status);
                tmp = ft_strjoin(result, status_str);
                free(status_str);
                free(result);
                result = tmp;
                i += 2;
                continue;
            }
            // Handle $$ - process ID
            if (str[i + 1] == '$')
            {
                char *pid_str;
                pid_str = ft_itoa(getpid());
                tmp = ft_strjoin(result, pid_str);
                free(result);
                result = tmp;
                i += 2;
                continue;
            }
            // Handle ${VAR}
            if (str[i + 1] == '{')
            {
                int start = i + 2;
                int j = start;
                while (str[j] && str[j] != '}')
                    j++;
                if (str[j] == '}' && j > start) // require at least one char
                {
                    char *var_name = ft_substr(str, start, j - start);
                    char *var_value = get_var_value(var_name, env, status);
                    free(var_name);
                    if (var_value)
                    {
                        tmp = ft_strjoin(result, var_value);
                        free(result);
                        result = tmp;
                        free(var_value);
                    }
                    i = j + 1; // move past the }
                    continue;
                }
                else
                {
                    // Invalid ${, treat as literal '$'
                    char curr[2] = {'$', '\0'};
                    tmp = ft_strjoin(result, curr);
                    free(result);
                    result = tmp;
                    i++;
                    continue;
                }
            }
            // Handle regular variable: [a-zA-Z_][a-zA-Z0-9_]*
            if (ft_isalpha(str[i + 1]) || str[i + 1] == '_')
            {
                int start = i + 1;
                i++; // move to first char of var name
                while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
                    i++;
                int len = i - start;
                char *var_name = ft_substr(str, start, len);
                char *var_value = get_var_value(var_name, env, status);
                free(var_name);
                if (var_value)
                {
                    tmp = ft_strjoin(result, var_value);
                    free(result);
                    result = tmp;
                    free(var_value);
                }
                continue;
            }
            // Single $ without valid variable name
            char curr[2] = {'$', '\0'};
            tmp = ft_strjoin(result, curr);
            free(result);
            result = tmp;
            i++;
        }
        else
        {
            char curr[2] = {str[i], '\0'};
            tmp = ft_strjoin(result, curr);
            free(result);
            result = tmp;
            i++;
        }
    }
    return result;
}

char	**expand(char **argv, char **env, int status)
{
	int		i;
	int		len;
	char	**expanded;
	char	*temp;

	if (!argv || !argv[0])
		return (NULL);
	len = 0;
	while (argv[len])
		len++;
	expanded = malloc(sizeof(char *) * (len + 1));
	if (!expanded)
		return (NULL);
	i = 0;
	while (i < len)
	{
		expanded[i] = expand_string(argv[i], env, status);
		i++;
	}
	expanded[i] = NULL;
	return (expanded);
}