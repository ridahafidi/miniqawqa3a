/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_spaces.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:42:40 by yel-qori          #+#    #+#             */
/*   Updated: 2025/05/19 19:36:19 by rhafidi          ###   ########.fr       */
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
    
    // Count needed spaces
    for (i = 0; i < len; i++)
    {
        if (is_operator_char(input[i]))
        {
            // Check for >> or << cases
            if (i + 1 < len && input[i] == input[i + 1])
            {
                new_len += 2; // Space before and after '>>' or '<<'
                i++;         // Skip next character since we handled it
            }
            else
            {
                new_len += 2; // Space before and after single operator
            }
        }
    }
    
    char *result = malloc(new_len + 1);
    if (!result)
        return NULL;
    
    int j = 0;
    for (i = 0; i < len; i++)
    {
        if (is_operator_char(input[i]))
        {
            // Handle >> and << cases
            if (i + 1 < len && input[i] == input[i + 1])
            {
                if (j > 0 && result[j-1] != ' ')
                    result[j++] = ' ';
                result[j++] = input[i];
                result[j++] = input[i+1];
                if (i + 2 < len && input[i+2] != ' ')
                    result[j++] = ' ';
                i++; // Skip next char since we handled it
                continue;
            }
            // Handle single operator
            if (j > 0 && result[j-1] != ' ')
                result[j++] = ' ';
            result[j++] = input[i];
            if (i + 1 < len && input[i+1] != ' ')
                result[j++] = ' ';
        }
        else
        {
            result[j++] = input[i];
        }
    }
    result[j] = '\0';
    
    return result;
}
