/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_spaces.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:42:40 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/05 17:22:19 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

char	*add_delimiter_spaces(char *input)
{
    int len = strlen(input);
    int new_len = len;
    int i;
    char quote = 0;
    
    // First pass: count needed spaces
    for (i = 0; i < len; i++) {
        if (input[i] == '\'' || input[i] == '"') {
            if (!quote)
                quote = input[i];
            else if (quote == input[i])
                quote = 0;
            continue;
        }
        
        if (!quote && is_operator_char(input[i])) {
            // Check for >> or << cases
            if (i + 1 < len && input[i] == input[i + 1]) {
                new_len += 2;
                i++;
            } else {
                new_len += 2;
            }
        }
    }
    
    char *result = malloc(new_len + 1);
    if (!result)
        return NULL;
    
    int j = 0;
    quote = 0;
    for (i = 0; i < len; i++) {
        if (input[i] == '\'' || input[i] == '"') {
            if (!quote)
                quote = input[i];
            else if (quote == input[i])
                quote = 0;
            result[j++] = input[i];
            continue;
        }
        
        if (!quote && is_operator_char(input[i])) {
            // Handle >> and << cases
            if (i + 1 < len && input[i] == input[i + 1]) {
                if (j > 0 && result[j-1] != ' ')
                    result[j++] = ' ';
                result[j++] = input[i];
                result[j++] = input[i+1];
                if (i + 2 < len && input[i+2] != ' ')
                    result[j++] = ' ';
                i++;
            } else {
                // Handle single operator
                if (j > 0 && result[j-1] != ' ')
                    result[j++] = ' ';
                result[j++] = input[i];
                if (i + 1 < len && input[i+1] != ' ')
                    result[j++] = ' ';
            }
        } else {
            result[j++] = input[i];
        }
    }
    result[j] = '\0';
    return result;
}