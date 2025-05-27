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

char **tokenize_input(char *input)
{
    char **tokens;
    int i = 0, j = 0, k = 0;
    int len = ft_strlen(input);

    // Allocate memory for tokens (worst-case scenario: each char is a token)
    tokens = malloc(sizeof(char *) * (len + 1));
    if (!tokens)
        return (NULL);

    while (input[i])
    {
        // Skip whitespace
        if (input[i] == ' ')
        {
            i++;
            continue;
        }

        // Handle multi-character operators like "<<", ">>"
        if (is_operator_char(input[i]))
        {
            if ((input[i] == '<' && input[i + 1] == '<') || 
                (input[i] == '>' && input[i + 1] == '>'))
            {
                tokens[j] = ft_substr(input, i, 2);
                if (!tokens[j++]) // Check for memory allocation failure
                {
                    free_token_array(tokens);
                    return (NULL);
                }
                i += 2;
            }
            else
            {
                tokens[j] = ft_substr(input, i, 1);
                if (!tokens[j++]) // Check for memory allocation failure
                {
                    free_token_array(tokens);
                    return (NULL);
                }
                i++;
            }
            continue;
        }

        // Handle regular tokens (e.g., "cat", "x")
        k = i;
        while (input[k] && !is_operator_char(input[k]) && input[k] != ' ')
            k++;
        tokens[j] = ft_substr(input, i, k - i);
        if (!tokens[j++]) // Check for memory allocation failure
        {
            free_token_array(tokens);
            return (NULL);
        }
        i = k;
    }

    tokens[j] = NULL; // Null-terminate the token array

    // Handle invalid syntax (e.g., unclosed quotes, invalid pipes)
    if (check_valid_quotes(input) == 0)
    {
        printf("minishell: unclosed quotes\n");
        free_token_array(tokens);
        return (NULL);
    }
    if (invalid_pipe(tokens) == 0)
    {
        printf("minishell: syntax error near unexpected token `|`\n");
        free_token_array(tokens);
        return (NULL);
    }

    // Strip quotes from tokens, but preserve quotes for heredoc delimiters
    strip_quotes_from_tokens(tokens, 1);

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

        if (token_len >= 2 && 
            ((token_str[0] == '"' && token_str[token_len - 1] == '"') ||
             (token_str[0] == '\'' && token_str[token_len - 1] == '\'')))
        {
            char *stripped = ft_substr(token_str, 1, token_len - 2);
            free(tokens[i]);
            tokens[i] = stripped;
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
