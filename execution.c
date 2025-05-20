/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:52:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/05/20 17:29:22 by rhafidi          ###   ########.fr       */
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
void    forker(t_tree *root, t_fd *fd, char ***env)
{
    int pid;
    int status;

    if (!is_builtin(root->command[0]))
    {
        redirecting(fd->in, fd->out);
        exit_status = handle_builtins(root, fd->in, fd->out, env, exit_status);
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
        signal(SIGINT, child_sigint_handler);  // or child_sigint_handler
        signal(SIGQUIT, ctrl_d_handle);
        redirecting(fd->in, fd->out);
        execute_command(root,fd->in, fd->out ,*env);
    }
    waitpid(pid, &exit_status, 0);
    exit_status = get_exit_status();
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
    close (pfd[0]);
    close (pfd[1]);
    waitpid(pid->left_pid, &exit_status, 0);
    waitpid(pid->right_pid, &exit_status, 0);
    free(pid);
}
void free_exit (t_pid *pid, t_fd *fd)
{
    free(pid);
    free(fd);
    exit(exit_status);
}

void handle_pipe(t_pid *pid, t_fd *fd, char ***env, t_tree *root)
{
    int pfd[2];
    
    pipein(pfd);
    pid->left_pid = fork();
    if (!pid->left_pid)
    {
        signal(SIGINT, child_sigint_handler);  // Add this
        signal(SIGQUIT, ctrl_d_handle);
        close(pfd[0]);
        fd->out = pfd[1];
        execution(root->left, fd, env);
        free_exit(pid, fd);
    }
    pid->right_pid = fork();
    if (!pid->right_pid)
    {
        signal(SIGINT, child_sigint_handler);  // Add this
        signal(SIGQUIT, ctrl_d_handle);
        close(pfd[1]);
        fd->in = pfd[0];
        execution(root->right, fd, env);
        free_exit(pid, fd);
    }
    close_wait(pfd, pid, fd);
}

void    execution(t_tree *root,t_fd *fd, char ***env)
{
    int pfd[2];
    t_pid *pid;

    pid = malloc(sizeof(t_pid));
    if (!pid)
        return ;
    if (!root)
    {
        free(pid);
        return ;
    }
    if (root->type == COMMAND)
        forker(root, fd , env);
    else if (root->type == PIPE)
        handle_pipe(pid , fd, env, root);
    else if (root->type == APPEND || root->type == GREATER || root->type == LESS)
    {
        handle_redirections(root, &fd->in, &fd->out);
        execution(root->left, fd, env);
        return ;
    }
}

int    initialize(t_tree *root, t_fd *fd, char ***env)
{
    signal(SIGINT, sigint_handler);
	signal(SIGQUIT, ctrl_d_handle);
    execution(root, fd, env);
    return (exit_status);
}


// int main(int ac, char **av, char **env)
// {
//     int stat;
//     t_fd *fds;
//     char    **my_env;
    
//     fds = malloc(sizeof(t_fd));
//     if (!fds)
//     {
//         perror("Memory allocation failed");
//         return (EXIT_FAILURE);
//     }
    
//     fds->in = STDIN_FILENO;
//     fds->out = STDOUT_FILENO;
//     my_env = copy_env(env);

//     // First export command
//     t_tree *export1 = malloc(sizeof(t_tree));
//     export1->left = export1->right = NULL;
//     export1->command = malloc(5 * sizeof(char *));
//     export1->command[0] = "cat";
//     // export1->command[1] = "xar1=xcasd";
//     export1->command[1] = NULL;
//     export1->type = COMMAND;

//     t_tree *export2 = malloc(sizeof(t_tree));
//     export2->left = export2->right = NULL;
//     export2->command = malloc(5 * sizeof(char *));
//     export2->command[0] = "ls";
//     // export2->command[1] = "PAH=ssss";
//     // export2->command[2] = "var2=dddd";
//     // export2->command[3] = "var3=xxxx";
//     export2->command[1] = NULL;
//     export2->type = COMMAND;

//      t_tree *export3 = malloc(sizeof(t_tree));
//     export3->left = export3->right = NULL;
//     export3->command = malloc(1 * sizeof(char *));
//     export3->command[0] = "unset";
//     export3->command[1] = "var2";
//     export3->command[2] = NULL;
//     export3->type = COMMAND;
    
//     t_tree *pipe = malloc(sizeof(t_tree));
//     pipe->left = export2;
//     pipe->right = export1;
//     pipe->command = NULL;
//     pipe->type = PIPE;
    
//     // stat = initialize_exec_wait(pipe, fds, &my_env); // Pass address of my_env
//     // printf("-----------------------------------------------------------------------");
//     // stat = initialize_exec_wait(export2, fds, &my_env); // Passaddress of my_env
//     stat = initialize(export1, fds, &my_env); // Passaddress of my_env
//     // stat = initialize_exec_wait(export3, fds, &my_env); // Pass address of my_env
//     //     stat = initialize_exec_wait(export1, fds, &my_env); // Passaddress of my_env

//     // stat = initialize_exec_wait(pipe, fds, &my_env); // Passaddress of my_env
//     // stat = initialize_exec_wait(export2, fds, &my_env); // Passaddress of my_env
//     // // stat = initialize_exec_wait(export3, fds, &my_env); // Passaddress of my_env
        



    
//     // Free memory
//     free_tree(&pipe);
//     free_array(my_env);
//     free(fds);
    
//     return (stat);
// }