/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
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

char **tokenize_input(char *input)
{
    char **tokens;
    int i = 0, j = 0, k = 0;
    int len = ft_strlen(input);

    tokens = malloc(sizeof(char *) * (len + 1)); // Allocate memory for tokens
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
                tokens[j++] = ft_substr(input, i, 2); // Extract "<<"/">>"
                i += 2;
            }
            else
            {
                tokens[j++] = ft_substr(input, i, 1); // Extract single operator
                i++;
            }
            continue;
        }

        // Handle regular tokens (e.g., "cat", "x")
        k = i;
        while (input[k] && !is_operator_char(input[k]) && input[k] != ' ')
            k++;
        tokens[j++] = ft_substr(input, i, k - i); // Extract token
        i = k;
    }

    tokens[j] = NULL; // Null-terminate the token array
    return tokens;
}
