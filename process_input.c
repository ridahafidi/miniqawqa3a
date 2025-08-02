/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 15:25:56 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 19:21:26 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_ast(t_tree *ast, char **tokens, t_data *data, int *exit_status)
{
	free_array(tokens);
	*exit_status = initialize(ast, data, exit_status);
	free_tree(&ast);
	return (0);
}

t_data	*init_data(int *exit_status)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	data->fds = malloc(sizeof(t_fd));
	if (!data->fds || !data)
	{
		perror("Memory allocation failed");
		*exit_status = EXIT_FAILURE;
		return (NULL);
	}
	return (data);
}

void	free_data(t_data *data)
{
	if (data->fds)
		free(data->fds);
	if (data)
		free(data);
}

int	process_cmd(char *input, char ***env, char ***exported, int *exit_status)
{
	char	**tokens;
	t_tree	*ast;
	t_data	*data;

	data = init_data(exit_status);
	if (!data)
		return (EXIT_FAILURE);
	data->fds->in = STDIN_FILENO;
	data->fds->out = STDOUT_FILENO;
	tokens = tokenize_input(input, *env, *exit_status);
	if (!tokens)
		return (handle_tokenization_error(data->fds, data, exit_status));
	ast = parse_tokens(tokens);
	data->env = env;
	data->exported = exported;
	if (ast)
		execute_ast(ast, tokens, data, exit_status);
	else
		return (handle_parsing_error(tokens, data, exit_status));
	free_data(data);
	return (0);
}

int	process_input_line(char *input, char ***env, char ***exported,
		int *exit_status)
{
	int	ret;

	if (input[0] != '\0')
	{
		add_history(input);
		ret = process_cmd(input, env, exported, exit_status);
		if (ret == 1)
			printf("\n");
		return (ret);
	}
	return (0);
}
