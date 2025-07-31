/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:27:23 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/31 18:45:05 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int process_cmd(char *input, char ***env, char ***exported, int *exit_status)
{
    t_fd *fds;
    char **tokens;
    t_tree *ast;

    fds = malloc(sizeof(t_fd));
    if (!fds)
    {
        perror("Memory allocation failed");
        *exit_status = EXIT_FAILURE;
        return (EXIT_FAILURE);
    }
    fds->in = STDIN_FILENO;
    fds->out = STDOUT_FILENO;
    tokens = tokenize_input(input, *env, *exit_status);
    if (!tokens)
    {
        free(fds);
        *exit_status = 2;  // Syntax error
        return (0);
    }
    ast = parse_tokens(tokens);
    if (ast)
    {
        free_array(tokens);
        /* Execute command and update exit status */
        *exit_status = initialize(ast, fds, env, exported, exit_status);
        free_tree(&ast);
    }
    else if (tokens)
    {
        free_array(tokens);
        *exit_status = 2;  // Syntax error
    }
    free(fds);
    return (0);
}

void    shell_loop(char ***env, char ***exported, int *exit_status)
{
    char    *input;
    char    exit;
    int     ret;

    exit = 0;
    while (!exit)
    {
        /* Reset signal handlers before each prompt to ensure consistent behavior */
        signal(SIGINT, sigint_handler);
        signal(SIGQUIT, SIG_IGN);
        
        // Check if a signal was received and update exit status accordingly
        check_received_signal(exit_status);
        
        input = readline("minishell> ");
        
        // Check if a signal was received during readline and update exit status immediately
        check_received_signal(exit_status);
        
        if (input == NULL) {
            free_array(env[0]);
            free_array(exported[0]);
            ctrl_d_handle(0, *exit_status);
        }
        if (input[0] != '\0')
        {
            add_history(input);
            ret = process_cmd(input, env, exported, exit_status);
            if (ret == 1)
                printf("\n");
            exit = ret;
        }
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

void    update_shlvl(char ***env)
{
    int i;
    int j;
    char    *new_shlvl;
    char    *val;
    int new_val;
    char    *tmp;

    i = 0;
    j = 0;
    if (!env[0][i])
        return ;
    while (env[0][i])
    {
        if (!strncmp("SHLVL", env[0][i], ft_strlen("SHLVL") - 1) && env[0][i][ft_strlen("SHLVL")] == '=')
        {
            new_val = ft_atoi(&env[0][i][ft_strlen("SHLVL") + 1]);
            new_val++;
            val = ft_itoa(new_val);
            new_shlvl = ft_strjoin("SHLVL=", val);
            (free(val), free(env[0][i]));
            env[0][i] = new_shlvl;
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
    
    // Initialize signal number to 0
    g_signum = 0;
    
    // Signal handlers
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
