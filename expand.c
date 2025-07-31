/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:32:41 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/25 21:48:38 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "minishell.h"

char    *get_before_dollar(char *str, int dollar_index)
{
    int i;
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

static void	process_dollar_sequence(char *str, char *result, int *i, int *ri)
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
	result = malloc(len * 2 + 1);  // Just need space for doubling $ characters at most
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

static char	*get_braced_var_name(char *str, int *i, int *len)
{
	char	*var_name;

	(*i)++;
	while (str[*i + *len] && str[*i + *len] != '}')
		(*len)++;
	if (str[*i + *len] != '}')
		return (NULL);
	var_name = malloc(sizeof(char) * (*len + 1));
	if (!var_name)
		return (NULL);
	*len = 0;
	while (str[*i] && str[*i] != '}')
		var_name[(*len)++] = str[(*i)++];
	var_name[*len] = '\0';
	return (var_name);
}

static char	*get_regular_var_name(char *str, int *i, int *len)
{
	char	*var_name;

	while (str[*i + *len] && (ft_isalnum(str[*i + *len]) || str[*i + *len] == '_'))
		(*len)++;
	if (*len == 0)
		return (NULL);
	var_name = malloc(sizeof(char) * (*len + 1));
	if (!var_name)
		return (NULL);
	*i = 1;
	*len = 0;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		var_name[(*len)++] = str[(*i)++];
	var_name[*len] = '\0';
	return (var_name);
}

char	*get_var_name(char *str)
{
	int	i;
	int	len;

	i = 1;
	len = 0;
	if (str[i] == '?')
		return (ft_strdup("?"));
	if (str[i] == '$')
		return (ft_strdup("$"));
	if (str[i] == '{')
		return (get_braced_var_name(str, &i, &len));
	return (get_regular_var_name(str, &i, &len));
}

char	*get_var_value(char *var_name, char **env, int status)
{
    int     i;
    char    *value;
    char    *status_str;

    if (!var_name)
        return ft_strdup("$");
    
    if (!ft_strcmp(var_name, "?"))
    {
        status_str = ft_itoa(status);
        return status_str;
    }

    if (!ft_strcmp(var_name, "$"))
    {
        return ft_strdup("$");
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

static char	*handle_single_quote(char *result, char *str, int *i, int heredoc)
{
	char	*tmp;
	char	curr[2];

	if (heredoc == 1)
	{
		(*i)++;
		return (result);
	}
	curr[0] = str[*i];
	curr[1] = '\0';
	tmp = ft_strjoin(result, curr);
	free(result);
	(*i)++;
	return (tmp);
}

static char	*handle_double_quote(char *result, char *str, int *i, int heredoc)
{
	char	*tmp;
	char	curr[2];

	if (heredoc == 1)
	{
		(*i)++;
		return (result);
	}
	curr[0] = str[*i];
	curr[1] = '\0';
	tmp = ft_strjoin(result, curr);
	free(result);
	(*i)++;
	return (tmp);
}

static char	*handle_double_dollar(char *result, int *i)
{
	char	*tmp;

	tmp = ft_strjoin(result, "$$");
	free(result);
	(*i) += 2;
	return (tmp);
}

static char	*process_single_quote_section(char *result, char *quoted_section)
{
	char	*tmp;

	tmp = ft_strjoin(result, quoted_section);
	free(result);
	return (tmp);
}

static char	*process_double_quote_section(char *result, char *quoted_section,
	char **env, int status)
{
	char	*expanded_section;
	char	*tmp;

	expanded_section = expand_string(quoted_section, env, status, 0);
	tmp = ft_strjoin(result, expanded_section);
	free(result);
	free(expanded_section);
	return (tmp);
}

static char	*handle_dollar_quote(char *result, char *str, int *i, char **env,
	int status)
{
	char	quote_char;
	int		j;
	int		total_len;
	char	*quoted_section;
	char	*tmp;

	quote_char = str[*i + 1];
	j = *i + 2;
	while (str[j] && str[j] != quote_char)
		j++;
	if (str[j] == quote_char)
	{
		total_len = j - *i + 1;
		quoted_section = ft_substr(str, *i + 1, total_len - 1);
		if (quote_char == '\'')
			result = process_single_quote_section(result, quoted_section);
		else
			result = process_double_quote_section(result, quoted_section, env, status);
		free(quoted_section);
		*i = j + 1;
		return (result);
	}
	else
	{
		tmp = ft_strjoin(result, "$");
		free(result);
		(*i)++;
		return (tmp);
	}
}

static char	*handle_exit_status(char *result, int *i, int status)
{
	char	*tmp;
	char	*status_str;
	int		real_status;

	if (WIFEXITED(status))
		real_status = WEXITSTATUS(status);
	else
		real_status = status;
	status_str = ft_itoa(real_status);
	tmp = ft_strjoin(result, status_str);
	free(status_str);
	free(result);
	*i += 2;
	return (tmp);
}

static char	*handle_braced_var(char *result, char *str, int *i, char **env,
	int status)
{
	int		start;
	int		j;
	char	*var_name;
	char	*var_value;
	char	*tmp;

	start = *i + 2;
	j = start;
	while (str[j] && str[j] != '}')
		j++;
	if (str[j] == '}' && j > start)
	{
		var_name = ft_substr(str, start, j - start);
		var_value = get_var_value(var_name, env, status);
		free(var_name);
		if (var_value)
		{
			tmp = ft_strjoin(result, var_value);
			free(result);
			result = tmp;
			free(var_value);
		}
		*i = j + 1;
		return (result);
	}
	else
	{
		tmp = ft_strjoin(result, "$");
		free(result);
		(*i)++;
		return (tmp);
	}
}

static char	*handle_regular_var(char *result, char *str, int *i, char **env,
	int status)
{
	int		start;
	int		len;
	char	*var_name;
	char	*var_value;
	char	*tmp;

	start = *i + 1;
	(*i)++;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	len = *i - start;
	var_name = ft_substr(str, start, len);
	var_value = get_var_value(var_name, env, status);
	free(var_name);
	if (var_value)
	{
		tmp = ft_strjoin(result, var_value);
		free(result);
		result = tmp;
		free(var_value);
	}
	return (result);
}

static char	*handle_literal_dollar(char *result, int *i)
{
	char	*tmp;

	tmp = ft_strjoin(result, "$");
	free(result);
	(*i)++;
	return (tmp);
}

static char	*handle_regular_char(char *result, char *str, int *i)
{
	char	*tmp;
	char	curr[2];

	curr[0] = str[*i];
	curr[1] = '\0';
	tmp = ft_strjoin(result, curr);
	free(result);
	(*i)++;
	return (tmp);
}

static char	*handle_dollar_cases(char *result, char *str, int *i, char **env,
	int status, int *doll)
{
	if (str[*i + 1] == '$')
	{
		result = handle_double_dollar(result, i);
		return (result);
	}
	if (str[*i + 1] == '\'' || str[*i + 1] == '"')
		return (handle_dollar_quote(result, str, i, env, status));
	if (str[*i + 1] == '?')
		return (handle_exit_status(result, i, status));
	if (str[*i + 1] == '{')
		return (handle_braced_var(result, str, i, env, status));
	if (ft_isalpha(str[*i + 1]) || str[*i + 1] == '_')
		return (handle_regular_var(result, str, i, env, status));
	return (handle_literal_dollar(result, i));
}

static char	*process_character(char *result, char *str, int *i, char **env,
	int status, int *doll, int *in_single_quote, int *in_double_quote,
	int heredoc)
{
	if (str[*i] == '\'' && !*in_double_quote)
	{
		*in_single_quote = !*in_single_quote;
		return (handle_single_quote(result, str, i, heredoc));
	}
	if (str[*i] == '"' && !*in_single_quote)
	{
		*in_double_quote = !*in_double_quote;
		return (handle_double_quote(result, str, i, heredoc));
	}
	if (str[*i] == '$' && (heredoc == 1 || !*in_single_quote))
		return (handle_dollar_cases(result, str, i, env, status, doll));
	return (handle_regular_char(result, str, i));
}

char *expand_string(char *str, char **env, int status, int heredoc)
{
	char	*result;
	int		i;
	int		in_single_quote;
	int		in_double_quote;
	int		doll;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	doll = 0; // Keeping this for compatibility with function signature
	while (str[i])
	{
		result = process_character(result, str, &i, env, status, &doll,
				&in_single_quote, &in_double_quote, heredoc);
	}
	return (result);
}

char	**expand(char **argv, char **env, int status)
{
	int		i;
	int		len;
	char	**expanded;

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
