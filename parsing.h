/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:24:33 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/31 17:35:37 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <readline/history.h>
# include <readline/readline.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

// External exit_status has been removed, now passed by reference

typedef struct s_token_type
{
	char				**tokens;
	char				quote;
	char				*merged;
	int					start;
	int					end;
	int					len;
	int					full_len;
	int					end_len;
}						t_token_type;

typedef struct s_segment
{
	char				*str;
	char				quote;
	int					was_quoted;
	struct s_segment	*next;
}						t_segment;

typedef enum t_type
{
	COMMAND,
	LESS,
	GREATER,
	APPEND,
	PIPE,
	HEREDOC,
}						t_type;

typedef struct s_token_params
{
	char				**env;
	int					exit_status;
}						t_token_params;

typedef struct s_tree
{
	struct s_tree		*left;
	struct s_tree		*right;
	char				*file_name;
	char				**command;
	t_type				type;
}						t_tree;

typedef struct s_token_data
{
	char				**tokens;
	int					token_count;
	int					is_delimiter;
}						t_token_data;

typedef struct s_quote_data
{
	char				*result;
	int					j;
	char				current_quote;
}						t_quote_data;

int						check_syntax_errors(char **tokens);
void					free_tokens_array(char **tokens, int token_count);
int						validate_redirections_syntax(char **tokens);
char					*create_token(char *input, int start, int end);
int						is_complete_quoted_token(char *token);
char					get_current_quote_state(char *input, int pos);
int						find_token_end(char *input, int start);
void					skip_whitespace(char *input, int *i);
char					**initial_tokenization_with_env(char *input, char **env,
							int exit_status);
void					set_append_redirection(t_tree *redir, char *file_name);
void					set_greater_redirection(t_tree *redir, char *file_name);
char					*remove_quotes_from_string(char *str, int index);
int						is_operator_char(char c);
void					strip_quotes_from_ast(t_tree *ast);
int						has_file_argument(int i, int end);
void					strip_quotes_from_command(t_tree *ast);
void					cleanup_cmd_args(char **cmd_args, int arg_count);
char					**process_tokens_loop(char **cmd_args, char **tokens,
							int start, int end);
void					set_less_redirection(t_tree *redir, char *file_name);
int						handle_empty_quotes_case(char *str, int index,
							t_quote_data *data);
char					*expand_string(char *str, char **env, int status,
							int heredoc);
char					**tokenize_input(char *input, char **env, int status);
char					*merge_tokens(char **tokens, int start, int end);
t_tree					*create_command(void);
void					print_tree(t_tree *root);
int						check_first_token_pipe(char **tokens);
void					print_syntax_error(char *token);
int						is_redirection(char *token);
int						calculate_new_length(char *input, int len);
char					update_quote_state(char current_char, char quote);
int						handle_single_operator(char *input, char *result, int i,
							int j);
int						handle_double_operator(char *input, char *result, int i,
							int j);
int						process_length_calculation(char *input, int i, int len,
							char quote);

int						is_redirections(char *tokens);
t_tree					*create_redirections(char *operator, char *file_name);
t_tree					*parse_tokens(char **tokens);
void					ft_print_tree(t_tree *root);
int						initialize_end_index(char **tokens, int end);
char					**collect_command_arguments(char **tokens, int start,
							int end);
t_tree					*create_command_node(char **cmd_args);
t_tree					*process_redirections(t_tree *cmd_node, char **tokens,
							int start, int end);
char					**process_quotes(char **tokens);
void					free_token_array(char **tokens);
char					**create_new_token_array(char **tokens,
							char *merged_token, int start, int end);
int						calculate_merged_length(char **tokens, int start,
							int end);
int						find_quote_end(char **tokens, int start, char quote);
int						is_quoted_token(char *token, char *quote);
char					**initial_tokenization(char *input);
char					*add_delimiter_spaces(char *input);
int						check_valid_quotes(char *input);
int						invalid_redirections(char **tokens);
int						check_redirection(char *token);
int						is_pipe(char *token);
int						invalid_pipe(char **tokens);
int						ft_strcmp(char *s1, char *s2);
int						invalid_parenthese(char *input);
int						special_characters(char *input);

#endif