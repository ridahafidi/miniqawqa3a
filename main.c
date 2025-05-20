/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:27:23 by yel-qori          #+#    #+#             */
/*   Updated: 2025/05/20 18:03:00 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int exit_status = 0;

int process_cmd(char *input, char ***env)
{
    t_fd *fds;
    char **tokens;
    t_tree *ast;

    fds = malloc(sizeof(t_fd));
    if (!fds)
    {
        perror("Memory allocation failed");
        return (EXIT_FAILURE);
    }
    fds->in = STDIN_FILENO;
    fds->out = STDOUT_FILENO;
    tokens = tokenize_input(input);
    if (!tokens)
        return (0);
    ast = parse_tokens(tokens);
    if (ast)
    {
        free_array(tokens);
        exit_status = initialize(ast, fds, env);
        free_tree(&ast);
    }
    else if (tokens)
        free_array(tokens);
    return (0);
}

void    shell_loop(char ***env)
{
    char    *input;
    char    exit;
    int     ret;

    exit = 0;
    while (!exit)
    {
        input = readline("minishell> ");
		if (input == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (input[0] != '\0')
		{
			add_history(input);
			ret = process_cmd(input, env);
			if (ret == 1)
				printf("\n");
			exit = ret;
		}
		if (input)
        {
            free(input);
            input = NULL;
        }
	}
	clear_history();
}

int main(int ac, char **av, char **env) 
{
    (void)av;
    (void)ac;
    signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
    char    **my_env;
    my_env = copy_env(env);
    shell_loop(&my_env);
    free_array(my_env);
}
