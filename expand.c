/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:32:41 by rhafidi           #+#    #+#             */
/*   Updated: 2025/05/30 17:33:32 by rhafidi          ###   ########.fr       */
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

char *expand_string(const char *str, char **env, int status)
{
    char    *result;
    char    *tmp;
    int     i;
    char    quote;
    int     real_status;

    if (!str)
        return (NULL);
    
    result = ft_strdup("");
    if (!result)
        return (NULL);

    i = 0;
    quote = 0;
    while (str[i])
    {
        if ((str[i] == '\'' || str[i] == '\"') && (!quote || quote == str[i]))
        {
            if (!quote)
                quote = str[i];
            else
                quote = 0;
            i++;
            continue;
        }
        
        // Handle variable expansion outside single quotes
        if (str[i] == '$' && quote != '\'')
        {
            // Handle $? - exit status
            if (str[i + 1] == '?')
            {
                if (WIFEXITED(status))
                {
                    real_status = WEXITSTATUS(status);
                }
                else
                {
                    real_status = status;
                }
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
                char pid_str[32];
                snprintf(pid_str, sizeof(pid_str), "%d", getpid());
                tmp = ft_strjoin(result, pid_str);
                free(result);
                result = tmp;
                i += 2;
                continue;
            }
            // Handle regular variables
            if (ft_isalpha(str[i + 1]) || str[i + 1] == '_')
            {
                int start = i + 1;
                while (str[i + 1] && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
                    i++;
                char *var_name = ft_substr(str, start, i - start + 1);
                char *var_value = NULL;
                int j = 0;
                
                while (env && env[j])
                {
                    if (!ft_strncmp(env[j], var_name, ft_strlen(var_name)) 
                        && env[j][ft_strlen(var_name)] == '=')
                    {
                        var_value = env[j] + ft_strlen(var_name) + 1;
                        break;
                    }
                    j++;
                }
                if (var_value)
                {
                    tmp = ft_strjoin(result, var_value);
                    free(result);
                    result = tmp;
                }
                free(var_name);
                i++;
                continue;
            }
            // Single $ without variable name
            if (!ft_isalnum(str[i + 1]) && str[i + 1] != '_' && str[i + 1] != '?' && str[i + 1] != '$')
            {
                tmp = ft_strjoin(result, "$");
                free(result);
                result = tmp;
                i++;
                continue;
            }
        }
        // Add regular character
        char curr[2] = {str[i], '\0'};
        tmp = ft_strjoin(result, curr);
        free(result);
        result = tmp;
        i++;
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