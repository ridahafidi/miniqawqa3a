/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+           */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:49:40 by yel-qori          #+#                #+#             */
/*   Updated: 2025/05/26 17:36:08 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_operator_char(char c)
{
    return (c == '<' || c == '>' || c == '|');
}

void free_token_array(char **tokens)
{
    int i = 0;

    if (!tokens)
        return;

    while (tokens[i])
    {
        free(tokens[i]);
        i++;
    }
    free(tokens);
}

char **tokenize_input(char *input, char **env, int exit_status)
{
    char **tokens;
    char *spaced_input;
    char **expanded_tokens;
    int i;
    
    if (!input || !*input)
        return NULL;

    // First check for syntax errors
    if (!check_valid_quotes(input))
    {
        printf("minishell: unclosed quotes\n");
        return NULL;
    }
    
    if (special_characters(input))
        return NULL;

    // Add spaces around operators
    spaced_input = add_delimiter_spaces(input);
    if (!spaced_input)
        return NULL;

    // Initial tokenization
    tokens = initial_tokenization(spaced_input);
    free(spaced_input);
    
    if (!tokens)
        return NULL;

    // Expand variables in tokens (except in single quotes and heredoc delimiters)
    i = 0;
    while (tokens[i])
    {
        // Skip expansion for heredoc delimiters
        if (i > 0 && ft_strcmp(tokens[i - 1], "<<") == 0)
        {
            i++;
            continue;
        }

        // Skip expansion inside single quotes
        if (tokens[i][0] == '\'')
        {
            i++;
            continue;
        }

        char *expanded = expand_string(tokens[i], env, exit_status);
        if (expanded)
        {
            free(tokens[i]);
            tokens[i] = expanded;
        }
        i++;
    }

    // Strip quotes after expansion
    strip_quotes_from_tokens(tokens, 1);  // 1 to skip heredoc delimiters

    // Check for pipe errors
    if (!invalid_pipe(tokens))
    {
        free_token_array(tokens);
        return NULL;
    }

    return tokens;
}

void strip_quotes_from_tokens(char **tokens, int skip_heredoc_delimiter)
{
    int i = 0;

    while (tokens[i])
    {
        char *token_str = tokens[i];
        size_t token_len = ft_strlen(token_str);

        // Skip stripping quotes for heredoc delimiters if specified
        if (skip_heredoc_delimiter && i > 0 && ft_strcmp(tokens[i - 1], "<<") == 0)
        {
            i++;
            continue;
        }

        // Remove surrounding quotes (both single and double)
        if (token_len >= 2)
        {
            if ((token_str[0] == '\'' && token_str[token_len - 1] == '\'') ||
                (token_str[0] == '\"' && token_str[token_len - 1] == '\"'))
            {
                // Create new string without quotes
                char *unquoted = ft_substr(token_str, 1, token_len - 2);
                free(tokens[i]);
                tokens[i] = unquoted;
            }
        }
        i++;
    }
}

char	*merge_tokens(char **tokens, int start, int end)
{
	char	*merged;
	char	*tmp;
	int		i;

	merged = ft_strdup("");
	i = start;
	while (i <= end)
	{
		tmp = ft_strjoin(merged, tokens[i]);
		free(merged);
		merged = tmp;
		if (i != end)
		{
			tmp = ft_strjoin(merged, " ");
			free(merged);
			merged = tmp;
		}
		i++;
	}
	return (merged);
}

char **initial_tokenization(char *input)
{
    char **tokens;
    
    if (!input)
        return (NULL);
    
    // Split input by whitespace using libft's ft_split
    tokens = ft_split(input, ' ');
    if (!tokens)
        return (NULL);
        
    // Check for redirection syntax errors
    if (!invalid_redirections(tokens))
    {
        free_token_array(tokens);
        return (NULL);
    }
    
    return tokens;
}

int invalid_redirections(char **tokens)
{
    int i;

    if (!tokens)
        return (0);

    i = 0;
    while (tokens[i])
    {
        // Check for redirection operators
        if (tokens[i][0] == '>' || tokens[i][0] == '<')
        {
            // Check if it's >> or << (valid double operators)
            if (tokens[i][1] == tokens[i][0] && tokens[i][2] == '\0')
            {
                if (!tokens[i + 1])
                {
                    ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
                    return (0);
                }
            }
            // Handle single > or < and invalid operators (>>> etc)
            else if (tokens[i][1] != '\0' && tokens[i][1] != tokens[i][0])
            {
                ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
                ft_putstr_fd(&tokens[i][1], 2);
                ft_putstr_fd("'\n", 2);
                return (0);
            }
            // Check for missing file name
            if (!tokens[i + 1])
            {
                ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
                return (0);
            }
            // Check for consecutive operators
            if (is_operator_char(tokens[i + 1][0]))
            {
                ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
                ft_putstr_fd(tokens[i + 1], 2);
                ft_putstr_fd("'\n", 2);
                return (0);
            }
        }
        i++;
    }
    return (1);
}
