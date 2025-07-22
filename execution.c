/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:52:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/21 22:02:28 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int get_exit_status()
{
    if (WIFEXITED(exit_status))
        return (WEXITSTATUS(exit_status));
    else if (WIFSIGNALED(exit_status))
        return (128 + WTERMSIG(exit_status)); 
    return (EXIT_FAILURE);
}

void redirecting(int in, int out)
{
    dup2(in, STDIN_FILENO);
    dup2(out, STDOUT_FILENO);
    if (in != STDIN_FILENO)
        close(in);
    if (out != STDOUT_FILENO)
        close(out);
}

void    forker(t_tree *root, t_fd *fd, char ***env, char ***exported)
{
    int pid;
    int status;
    int saved_stdin;
    int saved_stdout;
    
    if (!root->command || !root->command[0])
    {
        saved_stdin = dup(STDIN_FILENO);
        saved_stdout = dup(STDOUT_FILENO);
        redirecting(fd->in, fd->out);
        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdin);
        close(saved_stdout);
        return ;
    }
    if (!is_builtin(root->command[0]))
    {
        saved_stdin = dup(STDIN_FILENO);
        saved_stdout = dup(STDOUT_FILENO);
        redirecting(fd->in, fd->out);
        if (fd->in == -1 || fd->out == -1)
        {
            exit_status = 1;
            dup2(saved_stdin, STDIN_FILENO);
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdin);
            close(saved_stdout);
            return ;
        }
        exit_status = handle_builtins(root, fd, env, exported, exit_status);
        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdin);
        close(saved_stdout);
        return ;
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork failed :");
        exit(1);
    }
    if (!pid)
    {
        signal(SIGINT, child_sigint_handler); // Reset SIGINT for child
        signal(SIGQUIT, SIG_DFL);             // Reset SIGQUIT for child
        redirecting(fd->in, fd->out);
        if (fd->in == -1 || fd->out == -1)
        {
            // free(fd);
            free_array(*env);
            free_array(*exported);
            clear_history();
            free_tree(&root);
            exit(EXIT_FAILURE);
        }
        // free(fd);
        execute_command(root, fd, *env, *exported);
    }
    else
    {
        waitpid(pid, &exit_status, 0);
        if (WIFSIGNALED(exit_status))
        {
            if (WTERMSIG(exit_status) == SIGQUIT)
            {
                ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
                exit_status = 131;
            }
            else if (WTERMSIG(exit_status) == SIGINT)
            {
                ft_putstr_fd("\n", STDERR_FILENO);
                exit_status = 130;
            }
        }
        else if (WIFEXITED(exit_status))
            exit_status = WEXITSTATUS(exit_status);
        // Restore shell signal handlers
        signal(SIGINT, sigint_handler);
        signal(SIGQUIT, SIG_IGN);
                    // free_tree(&root);
    }
}

void    pipein(int *pfd)
{
    if (pipe(pfd) == -1)
    {
        perror("pipe failed");
        exit(1);
    }
}

void    close_wait(int *pfd, t_pid *pid, t_fd *fd)
{
    // printf("Here\n");
    close (pfd[0]);
    close (pfd[1]);
    waitpid(pid->left_pid, NULL, 0);
    waitpid(pid->right_pid, &exit_status, 0);
    exit_status = get_exit_status();
    free(pid);
}

void free_exit (t_pid *pid, t_fd *fd)
{
    free(pid);
    free(fd);
    exit(exit_status);
}

void handle_pipe( t_fd *fd, char ***env, char ***exported, t_tree *root)
{
    int pfd[2];
    t_pid *pid;

    pid = malloc(sizeof(t_pid));
    pipein(pfd);
    pid->left_pid = fork();
    // exit(1);
    // printf("pid == %d\n", pid->left_pid);
    if (!pid->left_pid)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        close(pfd[0]);
        fd->out = pfd[1];
        execution(root->left, fd, env, exported);
        free_array(*exported);
        free_array(*env);
        clear_history();
        free_tree(&root);
        // printf("freeeing\n");
        free_exit(pid, fd);

    }
    pid->right_pid = fork();
    // printf("pid == %d\n", pid->right_pid);
    if (!pid->right_pid)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        close(pfd[1]);
        fd->in = pfd[0];
        execution(root->right, fd, env, exported);
        free_array(*exported);
        free_array(*env);
        clear_history();
        free_tree(&root);
                    // printf("freeeing\n");

        free_exit(pid, fd);
        
    }
    // else
    // {
    //     free_array(*exported);
    //     free_array(*env);
    //     clear_history();
    //     free_tree(&root);
    //     free(pid);
        
    // }
    close_wait(pfd, pid, fd);
}

void    execution(t_tree *root,t_fd *fd, char ***env, char ***exported)
{
    int pfd[2];
    int ret;
    t_pid *pid;
    t_tree *cmd;

    // pid = malloc(sizeof(t_pid));
    pid = 0;
    // if (!pid)
    //     return ;
    if (!root)
    {
        free(pid);
        // free_tree(&root);
        return ;
    }
    if (root->type == APPEND || root->type == GREATER || root->type == LESS || root->type == HEREDOC)
    {
        cmd = handle_redirections(root, &fd->in, &fd->out, env[0]);
        // If redirections failed, clean up and return
        if (exit_status != 0)
        {
            if (fd->in != STDIN_FILENO)
                close(fd->in);
            if (fd->out != STDOUT_FILENO)
                close(fd->out);
            // free(pid);
            // free_tree(&root);
            return;
        }
        // Execute the command with the redirections applied
        if (cmd)
            forker(cmd, fd, env, exported);
        else
            forker(NULL, fd, env, exported); // Handle redirection-only commands
        // Clean up file descriptors
        if (fd->in != STDIN_FILENO)
            close(fd->in);
        if (fd->out != STDOUT_FILENO)
            close(fd->out);
        // free(pid);
        return ;
    }
    else if (root->type == COMMAND)
        forker(root, fd, env, exported);
    else if (root->type == PIPE)
        handle_pipe( fd, env, exported, root);
}

int    initialize(t_tree *root, t_fd *fd, char ***env, char ***exported)
{
    execution(root, fd, env, exported);
    return (exit_status);
}