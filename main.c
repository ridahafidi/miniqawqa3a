/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:27:23 by yel-qori          #+#    #+#             */
/*   Updated: 2025/08/01 19:59:31 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static int	handle_tokenization_error(t_fd *fds, t_data *data, int *exit_status)
{
	free(fds);
	free(data);
	*exit_status = 2;
	return (0);
}

static int	execute_ast(t_tree *ast, char **tokens, t_data *data, int *exit_status)
{
	free_array(tokens);
	*exit_status = initialize(ast, data , exit_status);
	free_tree(&ast);
	return (0);
}

static int	handle_parsing_error(char **tokens, t_data *data, int *exit_status)
{
	if (tokens)
	{
		free_array(tokens);
		*exit_status = 2;
	}
	if (data->fds)
		free(data->fds);
	if (data)
		free(data);
	return (0);
}

int process_cmd(char *input, char ***env, char ***exported, int *exit_status)
{
	char	**tokens;
	t_tree	*ast;
	t_data	*data;

	data = malloc(sizeof(t_data));
	data->fds = malloc(sizeof(t_fd));
	if (!data->fds || !data)
	{
		perror("Memory allocation failed");
		*exit_status = EXIT_FAILURE;
		return (EXIT_FAILURE);
	}
	data->fds->in = STDIN_FILENO;
	data->fds->out = STDOUT_FILENO;
	tokens = tokenize_input(input, *env, *exit_status);
	if (!tokens)
		return (handle_tokenization_error(data->fds, data, exit_status));
	ast = parse_tokens(tokens);
	data->env = env;
	data->exported = exported;
	if (ast)
	{
		execute_ast(ast, tokens, data, exit_status);
	}
	else
		return handle_parsing_error(tokens, data, exit_status);
	// Cleanup after successful execution
	if (data->fds)
		free(data->fds);
	if (data)
		free(data);
	return (0);
}

static void	setup_signals_and_check(int *exit_status)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	check_received_signal(exit_status);
}

static void	handle_null_input(char ***env, char ***exported, int exit_status)
{
	free_array(env[0]);
	free_array(exported[0]);
	ctrl_d_handle(0, exit_status);
}

static int	process_input_line(char *input, char ***env, char ***exported, 
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

void    shell_loop(char ***env, char ***exported, int *exit_status)
{
	char	*input;
	char	exit;
	int		ret;

	exit = 0;
	while (!exit)
	{
		setup_signals_and_check(exit_status);
		input = readline("minishell> ");
		check_received_signal(exit_status);
		if (input == NULL)
			handle_null_input(env, exported, *exit_status);
		ret = process_input_line(input, env, exported, exit_status);
		exit = ret;
		if (input)
			free(input);
		input = NULL;
	}
	clear_history();
}

char    **handle_env_i()
{
    char **my_env;
    char cwd[4096];

    my_env = malloc(sizeof(char *) * 4);
    if (!my_env)
        return (NULL);
    if (!getcwd(cwd, sizeof(cwd)))
    {
        perror("getcwd failed ");
        free(my_env);
        return (NULL);
    }
    my_env[0] = ft_strjoin("PWD=", cwd);
    my_env[1] = ft_strdup("SHLVL=1");
    my_env[2] = ft_strdup("_=/usr/bin/env");
    my_env[3] = NULL;
    return (my_env);
}

static void	update_shlvl_value(char ***env, int i)
{
	char	*new_shlvl;
	char	*val;
	int		new_val;

	new_val = ft_atoi(&env[0][i][ft_strlen("SHLVL") + 1]);
	new_val++;
	val = ft_itoa(new_val);
	new_shlvl = ft_strjoin("SHLVL=", val);
	(free(val), free(env[0][i]));
	env[0][i] = new_shlvl;
}

void    update_shlvl(char ***env)
{
	int	i;

	i = 0;
	if (!env[0][i])
		return ;
	while (env[0][i])
	{
		if (!strncmp("SHLVL", env[0][i], ft_strlen("SHLVL") - 1) 
			&& env[0][i][ft_strlen("SHLVL")] == '=')
		{
			update_shlvl_value(env, i);
			return ;
		}
		i++;
	}
}

int main(int ac, char **av, char **env) 
{
    (void)av;
    (void)ac;
    char **my_env;
    int exit_status = 0;
    
    g_signum = 0;
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
    my_env = copy_env(env);
    update_shlvl(&my_env);
    if (!my_env[0])
        my_env = handle_env_i();
    char **exported = copy_env(env);
    shell_loop(&my_env, &exported, &exit_status);
    free_array(my_env);
    free_array(exported);
    return exit_status;
}
