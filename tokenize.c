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

// Add this function to your tokenize.c file


int is_complete_quoted_token(char *token)
{
    if (!token || strlen(token) < 2)
        return (0);
    
    char first = token[0];
    char last = token[strlen(token) - 1];
    
    return ((first == '\'' || first == '"') && first == last);
}

int is_operator_char(char c)
{
    return (c == '<' || c == '>' || c == '|');
}

int is_operator_char2(char c, char *quote)
{
    if (*quote)
        return (0); // Ignore operators inside quotes
    return (c == '<' || c == '>' || c == '|');
}
int is_quoted_token(char *token, char *quote)
{
    if (!token || !quote)
        return (0);
    if (token[0] == '\'' || token[0] == '"')
    {
        *quote = token[0];
        return (1);
    }
    return (0);
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


// // Helper struct for segment
// typedef struct s_segment {
//     char *str;
//     char quote; // 0=unquoted, '\''=single, '"'=double
//     struct s_segment *next;
// } t_segment;

static void free_segments(t_segment *seg) {
    t_segment *tmp;
    while (seg) {
        tmp = seg->next;
        free(seg->str);
        free(seg);
        seg = tmp;
    }
}

// Improved: Tokenize input into segments with correct quote context for expansion
static t_segment *get_next_segment(const char *input, size_t *i) {
    size_t start = *i;
    char quote = 0;
    size_t len = 0;
    t_segment *seg;
    // If at a quote, start a quoted segment
    if (input[*i] == '\'' || input[*i] == '"') {
        quote = input[(*i)++];
        start = *i;
        while (input[*i]) {
            if (input[*i] == quote)
                break;
            (*i)++;
        }
        len = *i - start;
        seg = malloc(sizeof(t_segment));
        seg->str = malloc(len + 1);
        if (len)
            memcpy(seg->str, input + start, len);
        seg->str[len] = 0;
        seg->quote = quote;
        seg->next = NULL;
        if (input[*i] == quote)
            (*i)++;
        return seg;
    } else {
        // Unquoted: collect until next quote or space
        start = *i;
        while (input[*i] && input[*i] != ' ' && input[*i] != '\'' && input[*i] != '"')
            (*i)++;
        len = *i - start;
        if (len == 0)
            return NULL;
        seg = malloc(sizeof(t_segment));
        seg->str = malloc(len + 1);
        if (len)
            memcpy(seg->str, input + start, len);
        seg->str[len] = 0;
        seg->quote = 0;
        seg->next = NULL;
        return seg;
    }
}


// Merge segments into a single string, expanding variables as needed
static char *merge_and_expand_segments(t_segment *head, char **env, int exit_status) {
    size_t total = 0;
    t_segment *seg = head;
    char *expanded, *result;
    // First, expand and count total length
    seg = head;
    while (seg) {
        if (seg->quote == '\'') {
            // Single-quoted: no expansion
            total += strlen(seg->str);
        } else {
            // Unquoted or double-quoted: expand
            expanded = expand_string(seg->str, env, exit_status);
            total += strlen(expanded);
            if (expanded != seg->str)
                free(expanded);
        }
        seg = seg->next;
    }
    result = malloc(total + 1);
    result[0] = 0;
    seg = head;
    while (seg) {
        if (seg->quote == '\'') {
            strcat(result, seg->str);
        } else {
            expanded = expand_string(seg->str, env, exit_status);
            strcat(result, expanded);
            if (expanded != seg->str)
                free(expanded);
        }
        seg = seg->next;
    }
    return result;
}

// Bash-like tokenizer: merges quoted/unquoted segments, splits only at unquoted spaces
char **initial_tokenization_with_env(char *input, char **env, int exit_status)
{
    char **tokens = malloc(sizeof(char *) * 1000); // Allocate enough space
    int token_count = 0;
    int i = 0;
    int start;
    char quote = 0;
    int  quote_type = -1;
    
    if (!tokens)
        return NULL;
    
    while (input[i])
    {
        // Skip spaces
        while (input[i] && input[i] == ' ')
            i++;
        
        if (!input[i])
            break;
            
        start = i;
        quote = 0;
        
        // Find end of current token, respecting quotes
        while (input[i])
        {
            if (input[i] == '\'' && !quote)
            {
                quote = input[i];
                quote_type = 0;
            }
            else if (input[i] == '"' && !quote)
            {
                quote = input[i];
                quote_type = 1;
            }
            else if (input[i] == quote)
                quote = 0;
            else if (!quote && input[i] == ' ')
                break;

            // printf("input[%d] == %c\n quote = %c\n quote_type == %d\n", i, input[i], quote, quote_type);
            i++;
        }
        
        // Extract token
        int len = i - start;
        tokens[token_count] = malloc(len + 1);
        if (!tokens[token_count])
        {
            // Free previously allocated tokens
            for (int j = 0; j < token_count; j++)
                free(tokens[j]);
            free(tokens);
            return NULL;
        }
        
        strncpy(tokens[token_count], input + start, len);
        tokens[token_count][len] = '\0';
        
        // Expand variables if needed
        if (quote_type != 0)
            tokens[token_count] = expand_string(tokens[token_count], env, exit_status);
        int x = 0;
        // printf("%s\n", tokens[token_count]);
        // while (x < token_count)
        // {
        //     printf("%s\n", tokens[x]);
        //     x++;
        // }
        
        token_count++;
    }
    
    tokens[token_count] = NULL;
    return tokens;
}

char **tokenize_input(char *input, char **env, int exit_status)
{
    char **tokens;
    char *spaced_input;
    if (!input || !*input)
        return NULL;
    if (!check_valid_quotes(input)) {
        printf("minishell: unclosed quotes\n");
        return NULL;
    }
    if (special_characters(input))
        return NULL;
    spaced_input = add_delimiter_spaces(input);
    if (!spaced_input)
        return NULL;
    tokens = initial_tokenization_with_env(spaced_input, env, exit_status);
    free(spaced_input);
    if (!tokens)
        return NULL;
    // Check for pipe errors
    if (!invalid_pipe(tokens)) {
        free_token_array(tokens);
        return NULL;
    }

    return tokens;
}


// Old initial_tokenization is not used anymore; see initial_tokenization_with_env
char **initial_tokenization(char *input) {
    // Not used
    (void)input;
    return NULL;
}

char *remove_quotes_from_string(char *str)
{
    int i, j;
    char *result;
    char current_quote;
    int len;
    
    if (!str)
        return NULL;
    
    len = ft_strlen(str);
    result = malloc(len + 1);
    if (!result)
        return NULL;
    
    i = 0;
    j = 0;
    current_quote = 0;
    
    while (str[i])
    {
        if (!current_quote && (str[i] == '\'' || str[i] == '"'))
        {
            current_quote = str[i];
            i++;
        }
        else if (current_quote && str[i] == current_quote)
        {
            current_quote = 0;
            i++;
        }
        else
        {
            result[j++] = str[i++];
        }
    }
    result[j] = '\0';
    return result;
}

void strip_quotes_from_tokens(char **tokens, int skip_heredoc_delimiter)
{
    int i;
    char *new_token;
    
    if (!tokens)
        return;
    
    i = 0;
    while (tokens[i])
    {
        if (skip_heredoc_delimiter && i == 1)
        {
            i++;
            continue;
        }
        
        new_token = remove_quotes_from_string(tokens[i]);
        if (new_token)
        {
            free(tokens[i]);
            tokens[i] = new_token;
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