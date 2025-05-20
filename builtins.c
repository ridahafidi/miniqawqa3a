/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 17:21:18 by rhafidi           #+#    #+#             */
/*   Updated: 2025/05/20 17:24:02 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_strcmp(char *s1, char *s2)
{
    int i;

    i = 0;
    while(s1[i])
    {
        if (s1[i] != s2[i])
            return(s1[i] - s2[i]);
        i++;
    }
    return (s1[i] - s2[i]);
}

int is_builtin(char *cmd)
{
    if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "pwd") ||
        !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env") ||
        !ft_strcmp(cmd, "exit"))
        return(0);
    return (1);
}
//echo 
// still need to handle echo $variable or echo $?

int compare_var_env(char *arg, char **env)
{
    int i;
    int j;
    int k;
    int found;

    i = 0;
    found = 0;
    if (arg[i] == '?')
        return (-2);
    while (env[i])
    {
        j = 0;
        k = 0;
        found = 0;
        while(arg[j])
        {
            if (env[i][k] == arg[j])
                found++;
            k++;   
            j++;
        }
        if (ft_strlen(arg) == found && env[i][k] == '=')
            return (i);
        i++;
    }
    return(-1);
}

int find_start(char *s)
{
    int i;

    i = 0;
    while (s[i])
    {
        if (s[i] == '=')
            return (i + 1);
        i++;
    }   
    return(0); 
}
int    hanlde_index (int index, char ***env, int status)
{
    if (index == -2)
    {
        ft_putnbr_fd(status, STDOUT_FILENO);
        return (EXIT_SUCCESS);
    }
    else if (index != -1)
    {
        char *str = ft_substr(env[0][index], find_start(env[0][index]), ft_strlen(env[0][index]));
        ft_putstr_fd(str, STDOUT_FILENO);
        return (EXIT_SUCCESS);
    }
    return (EXIT_FAILURE);
}

void    print_arguments(char **argv, int i, char ***env, int status)
{
    int     index;
    char    *dollar;
    int     j;
    int     space_flag;
    
    while (argv[i])
    {
        j = 0;
        space_flag = 0;
        while (argv[i][j])
        {
            if (argv[i][j] == '$')
            {
                index = compare_var_env(&argv[i][++j], env[0]);
                if (hanlde_index(index, env, status))
                    space_flag = 1;
                break;
            }
            write (STDOUT_FILENO, &argv[i][j], 1);
            j++;
        }
        i++;
        if (argv[i] && !space_flag)
            write(STDOUT_FILENO, " ", 1);
    }
}

int ft_echo(char **argv, char ***env, int status)
{
    int i;
    int n_flag;
    
    i = 1;
    n_flag = 0;
    if (!argv[i])
    {
        write(STDOUT_FILENO, "\n", 1);
        return (0);
    }
    else if (!ft_strcmp(argv[1], "-n"))
    {
        n_flag = 1;
        i++;
    }
    print_arguments(argv, i, env, status);
    if (n_flag == 0)
        write(STDOUT_FILENO, "\n", 1);
    return (0);
}

// cd
int check_dir_x(struct stat file_stat, char *path)
{
    if (stat(path, &file_stat) == -1)
    {
        perror("stat failed");
        return (0);
    }
    return ((file_stat.st_mode & S_IXUSR) && (S_ISDIR(file_stat.st_mode)));
}

char    *fetch_val(char **env, char *arg)
{
    int index;
    char    *value;

    index = compare_var_env(arg, env);
    if (index >= 0)
    {
        value = ft_strdup(&env[index][find_equal(env[index])]);
        ft_putstr_fd(value, STDOUT_FILENO);
        return (value);
    }
    return (NULL);
}

int ft_cd(char **argv, char **env)
{
    char  *path;
    struct stat file_stat;
    char    cwd[4096];
    char    new_cwd[4096];
    
    path = argv[1];
    if (ft_strchr(path, '$'))
        path = fetch_val(env, argv[1]);
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd failed");
        return (1);
    }
    if (!path)
    {
        path = getenv("HOME");
        if (!path)
        {
            perror("getenv failed");
            return (1);
        }
        if (check_dir_x(file_stat, path))
        {    
            if (chdir(path) == -1)
            {
                perror("chdir failed");
                return (1);
            }
        }
        else
        {
            perror("not dir or permession denied");
            return(1);
        }
    }
    else if (!ft_strcmp(path, "-"))
    {
        path = getenv("OLDPWD");
        if (!path)
        {
            perror("getenv failed");
            return (1);
        }
        if (check_dir_x(file_stat, path))
        {    
            if (chdir(path) == -1)
            {
                perror("chdir failed");
                return (1);
            }
        }
        else
        {
            perror("not dir or permession denied");
            return(1);
        }
    }
    else
    {

        if (check_dir_x(file_stat, path))
        {
            if (chdir(path) == -1)
            {
                perror("chdir failed");
                return (1);
            }
        }
        else
        {
            perror("not dir or permession denied");
            return(1);
        }
    }
    if (getcwd(new_cwd, sizeof(new_cwd)) == NULL)
    {
        perror("getcwd failed");
        return (1);
    }
    setenv("PWD", new_cwd, 1);
    setenv("OLDPWD", cwd, 1);
    return (0);
}
//start pwd implemetation:

int ft_pwd(char **argv)
{
    char cwd[4096];
    
    if (!argv[1])
    {
        if (!getcwd(cwd, sizeof(cwd)))
        {
            perror("getcwd failed ");
            return (1);
        }
        ft_putstr_fd(cwd, STDOUT_FILENO);
        ft_putstr_fd("\n", STDOUT_FILENO);
        return (0);
    }
    return (EXIT_FAILURE);
}
//export 

char    **copy_env(char **env)
{
    int len;
    char **copy;
    int i;

    len = 0;
    i = 0;
    while (env[len])
        len++;
    copy = malloc(sizeof(char *) * (len + 1));
    if (!copy)
        return (NULL);
    copy[len] = NULL;
    while (env[i])
    {
        copy[i] = ft_strdup(env[i]);
        i++;
    }
    return (copy);
}
void    ft_swap(char **export, int i, int j)
{
    char *tmp;
    
    if (export)
    {
        tmp = export[i];
        export[i] = export[j];
        export[j] = tmp;
    }
}

void sort_export(char **export)
{
    int i;
    int len;
    int j;
    int swapped;

    len = 0;
    while (export[len])
        len++;
    i = 0;
    while (i < len - 1)
    {
        swapped = 0;
        j = 0;
        while (j < len - i - 1)
        {
            if (ft_strcmp(export[j], export[j + 1]) > 0)
            {
                ft_swap(export, j, j + 1);
                swapped = 1;
            }
            j++;
        }
        i++;
    }
}

char    *copy_var(char **export, int i)
{
    char    *variable_name;
    int len;
    int j;

    len = 0;
    while (export[i][len])
    {
        if (export[i][len] == '=')
        {
            len++;
            break;
        }
        len++;
    }
    variable_name = malloc(sizeof(char ) * len + 1);
    ft_strlcpy(variable_name, export[i], len + 1);
    return (variable_name);
}

void    handle_quotes(char **export, int i, int j)
{
    char    *value;
    char    *variable_name;
    char    *quote;
    char *tmp;
    
    value = ft_strdup(&export[i][j + 1]);
    tmp = value;
    quote = "\"";
    value = ft_strjoin(quote, value);
    free(tmp);
    tmp = value;
    value = ft_strjoin(value, quote);
    free(tmp);
    variable_name = copy_var(export, i);
    free(export[i]);
    export[i] = ft_strjoin(variable_name, value);
}

void    print_format(char **export)
{
    int i;
    int j;
    char    *declare;

    i = 0;
    j = 0;
    declare = "declare -x ";
    while (export[i])
    {
        export[i] = ft_strjoin(declare, export[i]);
        j = 0;
        while (export[i][j])
        {
            if (export[i][j] == '=')
            {
                handle_quotes(export, i ,j);
                break;
            }
            j++;
        }
        i++;
    }
}

int    apply_add(char *new_var, char ***env)
{
    int len;
    int i;
    char    **new_env;

    len = 0;
    while ((*env)[len])
        len++;
    new_env = malloc((len + 2) * sizeof(char *));
    if (!new_env)
        return (1);
    i = 0;
    while (i < len)
    {
        new_env[i] = ft_strdup((*env)[i]);
        i++;
    }
    new_env[i] = ft_strdup(new_var);
    new_env[i + 1] = NULL;
    free_array(*env);
    *env = new_env;
    return (EXIT_SUCCESS);
}

int find_equal(char *str)
{
    int i;

    i = 0;
    while (str[i] != '=')
        i++;
    return (i);
}

int indetical_variable(char ***env, char *var)
{
    int j;
    int i;


    j = 0;
    while (env[0][j])
    {
        if (!ft_strncmp(var, env[0][j], find_equal(env[0][j])))
        {
            if (var[find_equal(var) - 1] == '+')
            {
                env[0][j] = ft_strjoin(env[0][j], var + find_equal(var) + 1);
                return (0);
            }
            free(env[0][j]);
            env[0][j] = ft_strdup(var);
            return (0);
        }
        j++;
    }
    return (1);
}

int    add_var(char **argv, char ***env)
{
    int i;
    int j;
    char    *new_var;

    i = 1;
    j = 0;
    while (argv[i])
    {
        new_var = argv[i];
        if (new_var[0] >= '0' && new_var[0] <= '9')
        {
            perror("not a valid identifier");
            return (EXIT_FAILURE);
        }
        if (indetical_variable(env, argv[i]))
        {
            if (apply_add(new_var, env))
                return(EXIT_FAILURE);    
        }
        i++;
    }
    return (EXIT_SUCCESS);
}
int ft_export(char **argv, char ***env)
{
    char **export;
    int i;

    i = 0;
    if (!argv[1])
    {
        export = copy_env(*env);
        sort_export(export);
        print_format(export);
        while(export[i])
        {
            ft_putstr_fd(export[i], STDOUT_FILENO);
            ft_putstr_fd("\n", STDOUT_FILENO);
            i++;
        }
        free(export);
        return(EXIT_SUCCESS);
    }
    else
        return (add_var(argv, env));
    return (EXIT_FAILURE);
}   
// u have to fix other export things

void    ft_swap_ptr(char ***env, int j, int len)
{
    char *tmp;

    tmp = env[0][j];
    env[0][j] = env[0][len];
    env[0][len] = tmp;
}

int ft_unset(char **argv, char ***env)
{
    int i;
    int j;
    int len;

    i = 1;
    while(env[0][len])
        len++;
    while (argv[i])
    {
        j = 0;
        while (env[0][j])
        {
            if (!ft_strncmp(argv[i], env[0][j], ft_strlen(argv[i])))
            {
                ft_swap_ptr(env, j, len-1);
                free(env[0][len-1]);
                env[0][len-1] = NULL;
                len--;
                break;
            }
            j++;
        }
        i++;
    }
    return(EXIT_SUCCESS);
}
// env

int ft_env(char **argv, char **env)
{
    int i;
    
    i = 0;
    if (!env || argv[1])
        return (EXIT_FAILURE);
    while (env[i])
    {
        ft_putstr_fd(env[i], STDOUT_FILENO);
        write (STDOUT_FILENO, "\n", 1);
        i++;
    }
    return (EXIT_SUCCESS);
}

int    ft_exit(char **argv)
{
    int i;
    int exit_code;

    i = 0;
    //check if numeric
    while (argv[1][i])
    {
        if ((argv[1][i] < '0' || argv[1][i] > '9') && argv[1][i] != '+' && argv[1][i] != '-')
            {
                ft_putstr_fd("exit : ", STDOUT_FILENO);
                ft_putstr_fd(argv[1], STDOUT_FILENO);
                ft_putstr_fd(":numeric argument required\n", STDOUT_FILENO);
                exit(255);
            }
        i++;
    }
    if (argv[1] && argv[2])
    {
        ft_putstr_fd("exit: too many arguments", STDOUT_FILENO);
        return (EXIT_FAILURE);
    }
    if (argv[1])
        exit_code = ft_atoi(argv[1]);
    else
        exit_code = 0;
    exit (exit_code & 0xFF);
}

int handle_builtins(t_tree *root, int in, int out, char ***env, int status)
{
    int exit_code;

    exit_code = status;
    if (!ft_strcmp(root->command[0], "echo"))
        exit_code = ft_echo(root->command, env, status);
    else if (!ft_strcmp(root->command[0], "cd"))
        exit_code = ft_cd(root->command, env[0]);
    else if (!ft_strcmp(root->command[0], "pwd"))
        exit_code = ft_pwd(root->command);
    else if (!ft_strcmp(root->command[0], "export"))
        exit_code = ft_export(root->command,env);
    else if (!ft_strcmp(root->command[0], "unset"))
        exit_code = ft_unset(root->command, env);
    else if (!ft_strcmp(root->command[0], "env"))
        exit_code = ft_env(root->command, env[0]);
    else if (!ft_strcmp(root->command[0], "exit"))
        exit_code = ft_exit(root->command);
    return (exit_code);
}
