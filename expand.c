/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:32:41 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/19 16:26:51 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *get_before_dollar(char *str, int dollar_index)
{
    int i;
    int len;
    char    *res;
    
    i = 0;
    res = malloc(sizeof(char) * dollar_index);
    if (!res || !dollar_index)
        return(NULL);
    while(i <= dollar_index)
    {
        res[i] = str[i];
        i++;
    }
    res[dollar_index] = '\0';
    return (res);
}

char	*get_pid_str(void)
{
	pid_t	pid;
	char	*pid_str;

	pid = getpid();
	pid_str = ft_itoa(pid);
	return (pid_str);
}

char	*expand_dollars(char *str)
{
	char	*result;
	char	*pid;
	int		i = 0, ri = 0;
	size_t	len = ft_strlen(str);

	result = malloc(len * 20 + 1);
	if (!result)
		return (NULL);
	pid = get_pid_str();
	while (str[i])
	{
		if (str[i] == '$')
		{
			int dollar_count = 0;
			while (str[i + dollar_count] == '$')
				dollar_count++;
			int pairs = dollar_count / 2;
			int rest = dollar_count % 2;
			while (pairs--)
			{
				ft_strlcpy(result + ri, pid, ft_strlen(pid) + 1);
				ri += ft_strlen(pid);
			}
			if (rest)
				result[ri++] = '$';
			i += dollar_count;
		}
		else
			result[ri++] = str[i++];
	}
	result[ri] = '\0';
	free(pid);
    // if (str)
    //     free(str);
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

char *expand_string(char *str, char **env, int status, int heredoc)
{
    char    *result;
    char    *tmp;
    int     i;
    int     real_status;
    int     in_single_quote;
    int     in_double_quote;
    int     doll = 0;

    if (!str)
        return (NULL);
    
    result = ft_strdup("");
    if (!result)
        return (NULL);

    i = 0;
    in_single_quote = 0;
    in_double_quote = 0;
    
    while (str[i])
    {
        // Handle quote state changes (but don't strip quotes unless heredoc mode)
        if (str[i] == '\'' && !in_double_quote)
        {
            in_single_quote = !in_single_quote;
            if (heredoc == 1) // In heredoc mode, skip quotes entirely
            {
                i++;
                continue;
            }
            // In normal mode, include the quote character
            char curr[2] = {str[i], '\0'};
            tmp = ft_strjoin(result, curr);
            free(result);
            result = tmp;
            i++;
            continue;
        }
        if (str[i] == '"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
            if (heredoc == 1) // In heredoc mode, skip quotes entirely
            {
                i++;
                continue;
            }
            // In normal mode, include the quote character
            char curr[2] = {str[i], '\0'};
            tmp = ft_strjoin(result, curr);
            free(result);
            result = tmp;
            i++;
            continue;
        }
        // Handle variable expansion
        // For heredoc: expand everything (ignore quotes)
        // For normal: only expand if not in single quotes
        if (str[i] == '$' && (heredoc == 1 || !in_single_quote))
        {
            if (str[i + 1] == '$' )
            {
                // printf("str == %s\n", str);
                str = expand_dollars(str);
                doll = 1;
                // printf("str == %s\n", str);
                // printf("res == %s\n", result);
                continue;
            }
            // Replace the problematic section with this:
            if (str[i + 1] == '\'' || str[i + 1] == '"')
            {
                char quote_char = str[i + 1];
                int j = i + 2; // Start after 
                
                // Find the closing quote
                while (str[j] && str[j] != quote_char)
                    j++;
                
                if (str[j] == quote_char) // Found closing quote
                {
                    // Extract the entire quoted section including quotes: content' or $"content"
                    int total_len = j - i + 1; // from $ to closing quote
                    char *quoted_section = ft_substr(str, i + 1, total_len - 1); // Skip the $, keep quotes
                    
                    if (quote_char == '\'') 
                    {
                        // For ...', keep quotes and treat content as literal
                        tmp = ft_strjoin(result, quoted_section);
                        free(result);
                        result = tmp;
                    }
                    else // quote_char == '"'
                    {
                        // For $"...", keep quotes but expand variables inside
                        char *expanded_section = expand_string(quoted_section, env, status, 0);
                        tmp = ft_strjoin(result, expanded_section);
                        free(result);
                        result = tmp;
                        free(expanded_section);
                    }
                    
                    free(quoted_section);
                    i = j + 1; // Move past the closing quote
                    continue;
                }
                else
                {
                    // No closing quote found, treat as literal $ + quote
                    char curr[2] = {'$', '\0'};
                    tmp = ft_strjoin(result, curr);
                    free(result);
                    result = tmp;
                    i++;
                    continue;
                }
            }
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
            // Regular character (including $ inside single quotes)
            char curr[2] = {str[i], '\0'};
            tmp = ft_strjoin(result, curr);
            free(result);
            result = tmp;
            i++;
        }
        // printf("res == %s\n", result);
    }
    if (doll)
        free(str);
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
		expanded[i] = expand_string(argv[i], env, status, 1);
		i++;
	}
	expanded[i] = NULL;
	return (expanded);
}