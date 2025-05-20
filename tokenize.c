/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:49:40 by yel-qori          #+#    #+#             */
/*   Updated: 2025/05/19 19:36:38 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// splited tokenize_input func

// char	**initial_tokenization(char *input)
// {
// 	return (ft_split(input, ' '));
// }

// int	is_quoted_token(char *token, char *quote)
// {
// 	int	len;

// 	if (token[0] == '"' || token[0] == '\'')
// 	{
// 		*quote = token[0];
// 		len = ft_strlen(token);
// 		if (token[len - 1] == *quote && len > 1)
// 			return (0);
// 		return (1);
// 	}
// 	return (0);
// }

// int	find_quote_end(char **tokens, int start, char quote)
// {
// 	int	end;
// 	int	end_len;

// 	end = start;
// 	while (tokens[end])
// 	{
// 		end_len = ft_strlen(tokens[end]);
// 		if (tokens[end][end_len - 1] == quote)
// 			break ;
// 		end++;
// 	}
// 	return (end);
// }

// int	calculate_merged_length(char **tokens, int start, int end)
// {
// 	int	full_len;
// 	int	k;

// 	full_len = 0;
// 	k = start;
// 	while (k <= end)
// 	{
// 		full_len += ft_strlen(tokens[k]) + 1;
// 		k++;
// 	}
// 	return (full_len);
// }

// char	**create_new_token_array(char **tokens, char *merged_token, int start,
// 		int end)
// {
// 	int		new_size;
// 	char	**new_tokens;

// 	int i, k;
// 	new_size = 0;
// 	while (tokens[new_size])
// 		new_size++;
// 	new_size = new_size - (end - start);
// 	new_tokens = malloc(sizeof(char *) * (new_size + 1));
// 	if (!new_tokens)
// 		return (NULL);
// 	k = 0;
// 	while (k < start)
// 	{
// 		new_tokens[k] = ft_strdup(tokens[k]);
// 		k++;
// 	}
// 	new_tokens[start] = ft_strdup(merged_token);
// 	k = end + 1;
// 	i = start;
// 	while (tokens[k])
// 	{
// 		new_tokens[++i] = ft_strdup(tokens[k]);
// 		k++;
// 	}
// 	new_tokens[new_size] = NULL;
// 	return (new_tokens);
// }

// void	free_token_array(char **tokens)
// {
// 	int	k;

// 	k = 0;
// 	while (tokens[k])
// 	{
// 		free(tokens[k]);
// 		k++;
// 	}
// 	free(tokens);
// }

// char	**process_quotes(char **tokens)
// {
// 	int		i;
// 	char	quote;
// 	char	*merged_token;
// 	char	**new_tokens;
// 	int		start;
// 	int		end;
// 	int		full_len;

// 	i = 0;
// 	while (tokens[i])
// 	{
// 		if (is_quoted_token(tokens[i], &quote))
// 		{
// 			start = i;
// 			end = find_quote_end(tokens, start, quote);
// 			full_len = calculate_merged_length(tokens, start, end);
// 			merged_token = merge_tokens(tokens, start, end);
// 			new_tokens = create_new_token_array(tokens, merged_token, start,
// 					end);
// 			free_token_array(tokens);
// 			free(merged_token);
// 			tokens = new_tokens;
// 		}
// 		i++;
// 	}
// 	return (tokens);
// }

// char	**tokenize_input(char *input)
// {
// 	char	**tokens;

// 	tokens = initial_tokenization(input);
// 	if (!tokens)
// 		return (NULL);
// 	return (process_quotes(tokens));
// }

char	**tokenize_input(char *input)
{
	int				i;
	int				k;
	t_token_type	token;
	char			**new_tokens;
	int				merged;
	int				new_size;
	char *insert_spaces;

	i = 0;
	k = 0;
	merged = 0;
	insert_spaces = add_delimiter_spaces(input);
	token.tokens = ft_split(insert_spaces, ' ');
	while (token.tokens[i])
	{
		if (token.tokens[i][0] == '"' || token.tokens[i][0] == '\'')
		{
			token.quote = token.tokens[i][0];
			token.start = i;
			token.end = i;
			token.len = ft_strlen(token.tokens[i]);
			if (token.tokens[i][token.len - 1] == token.quote && token.len > 1)
			{
				i++;
				continue ;
			}
			token.end++;
			while (token.tokens[token.end])
			{
				token.end_len = ft_strlen(token.tokens[token.end]);
				if (token.tokens[token.end][token.end_len - 1] == token.quote)
					break ;
				token.end++;
			}
			token.full_len = 0;
			k = token.start;
			while (k <= token.end)
			{
				token.full_len += ft_strlen(token.tokens[k]) + 1;
				k++;
			}
			token.merged = merge_tokens(token.tokens, token.start, token.end);
			new_size = 0;
			while (token.tokens[new_size])
				new_size++;
			new_size = new_size - (token.end - token.start);
			new_tokens = malloc(sizeof(char *) * (new_size + 1));
			k = 0;
			while (k < token.start)
			{
				new_tokens[k] = ft_strdup(token.tokens[k]);
				k++;
			}
			new_tokens[token.start] = ft_strdup(token.merged);
			k = token.end + 1;
			while (token.tokens[k])
			{
				new_tokens[++token.start] = ft_strdup(token.tokens[k]);
				k++;
			}
			new_tokens[new_size] = NULL;
			k = 0;
			while (token.tokens[k])
			{
				free(token.tokens[k]);
				k++;
			}
			free(token.tokens);
			token.tokens = new_tokens;
			free(token.merged);
			merged = 1;
		}
		i++;
	}
	return (token.tokens);
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
