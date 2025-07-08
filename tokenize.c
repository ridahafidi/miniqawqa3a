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
char **initial_tokenization_with_env(char *input, char **env, int exit_status) {
    size_t len = ft_strlen(input);
    char **tokens = malloc(sizeof(char *) * (len + 2));
    int t = 0;
    size_t i = 0;
    while (i < len) {
        // Skip leading spaces
        while (i < len && input[i] == ' ')
            i++;
        if (i >= len)
            break;
        t_segment *head = NULL, *tail = NULL, *seg;
        while (i < len && input[i] != ' ') {
            seg = get_next_segment(input, &i);
            if (!seg)
                break;
            if (!head)
                head = seg;
            else
                tail->next = seg;
            tail = seg;
        }
        if (head) {
            char *merged = merge_and_expand_segments(head, env, exit_status);
            tokens[t++] = merged;
            free_segments(head);
        }
    }
    tokens[t] = NULL;
    // if (!invalid_redirections(tokens)) {
    //     free_token_array(tokens);
    //     return NULL;
    // }
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

// No-op: quotes are already removed during tokenization
void strip_quotes_from_tokens(char **tokens, int skip_heredoc_delimiter)
{
    (void)tokens;
    (void)skip_heredoc_delimiter;
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