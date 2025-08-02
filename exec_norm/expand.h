/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 16:40:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 16:47:09 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "../minishell.h"

typedef struct s_expand_context
{
	char	**env;
	int		status;
	int		*doll;
	int		*in_single_quote;
	int		*in_double_quote;
	int		heredoc;
	char	*result;
	int		*i;
}	t_expand_context;

typedef struct s_dollar_context
{
	char	**env;
	int		status;
	int		*doll;
}	t_dollar_context;

// Function declarations
char	*get_before_dollar(char *str, int dollar_index);
void	process_dollar_sequence(char *str, char *result, int *i, int *ri);
char	*expand_dollars(char *str);
int		check_for_dollar(char *str);
char	*get_braced_var_name(char *str, int *i, int *len);
char	*get_regular_var_name(char *str, int *i, int *len);
char	*get_var_name(char *str);
char	*get_var_value(char *var_name, char **env, int status);
int		find_var_end(char *str);
char	*handle_single_quote(char *result, char *str, int *i, int heredoc);
char	*handle_double_quote(char *result, char *str, int *i, int heredoc);
char	*handle_double_dollar(char *result, int *i);
char	*process_single_quote_section(char *result, char *quoted_section);
char	*process_double_quote_section(char *result, char *quoted_section,
			char **env, int status);
char	*handle_dollar_quote(char *result, char *str, int *i,
			t_dollar_context *ctx);
char	*handle_exit_status(char *result, int *i, int status);
char	*handle_braced_var(char *result, char *str, int *i,
			t_dollar_context *ctx);
char	*handle_regular_var(char *result, char *str, int *i,
			t_dollar_context *ctx);
char	*handle_literal_dollar(char *result, int *i);
char	*handle_regular_char(char *result, char *str, int *i);
char	*handle_dollar_cases(char *result, char *str, int *i,
			t_dollar_context *ctx);
char	*process_character(char *result, char *str, int *i,
			t_expand_context *ctx);
char	*expand_string(char *str, char **env, int status, int heredoc);
char	**expand(char **argv, char **env, int status);

#endif
