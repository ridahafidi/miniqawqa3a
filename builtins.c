/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 17:21:18 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/05 17:16:06 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_swap_ptr(char ***arr, int i, int j)
{
    char *tmp = (*arr)[i];
    (*arr)[i] = (*arr)[j];
    (*arr)[j] = tmp;
}

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
    while (argv[i] && argv[i][0] == '-')
    {
        int j = 1;
        while (argv[i][j] == 'n')
            j++;
        if (argv[i][j] != '\0')  // If not all 'n', break
            break;
        if (j == 1)  // If just "-", break
            break;
        n_flag = 1;
        i++;
    }
    // strip_quotes_from_tokens(argv + i, 0);
    while (argv[i])
    {
        ft_putstr_fd(argv[i], STDOUT_FILENO);
        i++;
        if(argv[i])
           write(STDOUT_FILENO, " ", 1); 
    }
    if (n_flag == 0)
        write(STDOUT_FILENO, "\n", 1);
    return (0);
}

// cd
int check_dir_x(struct stat *file_stat, char *path)
{
    if (stat(path, file_stat) == -1)
    {
        perror("stat failed");
        return (0);
    }
    return ((file_stat->st_mode & S_IXUSR) && (S_ISDIR(file_stat->st_mode)));
}

void    update_env(char ***env, char *new_cwd, char *cwd)
{
    int     i;
    char    *tmp;
    char    *joiner;

    i = 0;
    while (env[0][i])
    {
        tmp = env[0][i];
        if (!ft_strncmp("PWD=", env[0][i], ft_strlen("PWD=")))
        {
            free(env[0][i]);
            joiner = ft_strjoin("PWD=", new_cwd);
            env[0][i] = joiner;
            joiner = NULL;
        }
        else if (!ft_strncmp("OLDPWD=", env[0][i], ft_strlen("OLDPWD=")))
        {
            free(env[0][i]);
            joiner = ft_strjoin("OLDPWD=", cwd);
            env[0][i] = joiner;
            joiner = NULL;
        }
        i++;
    }
}

int ft_cd(char **argv, char ***env)
{
    char  *path;
    char    **expanded;
    struct stat file_stat;
    char    cwd[4096];
    char    new_cwd[4096];
    
    if (argv[2])
        return (EXIT_FAILURE);
    if (argv[1])
        path = argv[1]; 
    else
        path = NULL;
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
        if (check_dir_x(&file_stat, path))
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
        if (check_dir_x(&file_stat, path))
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

        if (check_dir_x(&file_stat, path))
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
    update_env(env, new_cwd, cwd);
    return (0);
}
//start pwd implemetation:

int ft_pwd(char **argv)
{
    char cwd[4096];
    
    if (!getcwd(cwd, sizeof(cwd)))
    {
        perror("getcwd failed ");
        return (1);
    }
    ft_putstr_fd(cwd, STDOUT_FILENO);
    ft_putstr_fd("\n", STDOUT_FILENO);
    return (0);
}
//export 

int is_valid_identifier(char *str)
{
    int i;
    int flag_eq;

    i = 0;
    flag_eq = 0;
    if (!str || !*str)
        return (0);
    if (!ft_isalpha(str[i]) && str[i] != '_')
        return (0);
    i++;
    while (str[i])
    {
        if (str[i] == '=')
            flag_eq = 1;
        if (flag_eq == 0 && str[i] == '-')
            return (0);
        i++;
    }
    return (1);
}

char    **copy_env(char **env)
{
    int len;
    char **copy;
    int i;

    len = 0;
    i = 0;
    if (!env)
        return (NULL);
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

// Helper: check if var is in exported
int is_in_exported(char *var, char **exported) {
    int i = 0;
    int len = 0;
    while (var[len] && var[len] != '=')
        len++;
    while (exported && exported[i]) {
        if (!ft_strncmp(exported[i], var, len) &&
            (exported[i][len] == '\0' || exported[i][len] == '=')) {
            return i;
        }
        i++;
    }
    return -1;
}

// Helper: add or update var in exported
void add_or_update_exported(char *var, char ***exported) {
    int idx = is_in_exported(var, *exported);
    int len = 0, i = 0;
    while (*exported && (*exported)[len])
        len++;
    if (idx >= 0) {
        free((*exported)[idx]);
        (*exported)[idx] = ft_strdup(var);
    } else {
        char **new_exported = malloc(sizeof(char*) * (len + 2));
        for (i = 0; i < len; i++)
            new_exported[i] = (*exported)[i];
        new_exported[len] = ft_strdup(var);
        new_exported[len+1] = NULL;
        if (*exported)
            free(*exported);
        *exported = new_exported;
    }
}

// Helper: add or update var in env (only if contains '=')
void add_or_update_env(char *var, char ***env) {
    if (!ft_strchr(var, '='))
        return;
    int i = 0, len = 0;
    while (*env && (*env)[len])
        len++;
    int eqpos = 0;
    while (var[eqpos] && var[eqpos] != '=')
        eqpos++;
    for (i = 0; i < len; i++) {
        if (!ft_strncmp((*env)[i], var, eqpos) && (*env)[i][eqpos] == '=') {
            free((*env)[i]);
            (*env)[i] = ft_strdup(var);
            return;
        }
    }
    char **new_env = malloc(sizeof(char*) * (len + 2));
    for (i = 0; i < len; i++)
        new_env[i] = (*env)[i];
    new_env[len] = ft_strdup(var);
    new_env[len+1] = NULL;
    if (*env)
        free(*env);
    *env = new_env;
}

// Update add_var to handle exported/env split
void add_var(char **argv, char ***env, char ***exported) {
    int i = 1;
    while (argv[i])
    {
        if (!is_valid_identifier(argv[i])) {
            ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
            exit_status = 1;
        }
        else
        {
            if (ft_strchr(argv[i], '='))
            {
                add_or_update_exported(argv[i], exported);
                add_or_update_env(argv[i], env);
            }
            else
            {
                add_or_update_exported(argv[i], exported);
            }
            exit_status = 0;
        }
        i++;
    }
    // return (EXIT_SUCCESS);
}

// Update ft_export to print exported list or add variables
int ft_export(char **argv, char ***env, char ***exported) {
    int i = 0;
    if (!argv[1]) {
        char **copy = copy_env(*exported);
        sort_export(copy);
        print_format(copy);
        while (copy[i]) {
            ft_putstr_fd(copy[i], STDOUT_FILENO);
            ft_putstr_fd("\n", STDOUT_FILENO);
            i++;
        }
        free(copy);
        return (EXIT_SUCCESS);
    } else {
        add_var(argv, env, exported);
        return (exit_status);
    }
    return (EXIT_FAILURE);
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
        if (ft_strchr(env[i], '='))
            ft_putstr_fd(env[i], STDOUT_FILENO);
        write(STDOUT_FILENO, "\n", 1);
        i++;
    }
    return (EXIT_SUCCESS);
}

// Update ft_unset to remove from both env and exported
int ft_unset(char **argv, char ***env, char ***exported) {
    int i = 1, j, len;
    while (argv[i]) {
        // Remove from env
        len = 0;
        while ((*env)[len])
            len++;
        for (j = 0; j < len; j++) {
            int n = ft_strlen(argv[i]);
            if (!ft_strncmp(argv[i], (*env)[j], n) && ((*env)[j][n] == '=' || (*env)[j][n] == '\0')) {
                ft_swap_ptr(env, j, len-1);
                free((*env)[len-1]);
                (*env)[len-1] = NULL;
                len--;
                break;
            }
        }
        // Remove from exported
        len = 0;
        while ((*exported)[len])
            len++;
        for (j = 0; j < len; j++) {
            int n = ft_strlen(argv[i]);
            if (!ft_strncmp(argv[i], (*exported)[j], n) && ((*exported)[j][n] == '=' || (*exported)[j][n] == '\0')) {
                ft_swap_ptr(exported, j, len-1);
                free((*exported)[len-1]);
                (*exported)[len-1] = NULL;
                len--;
                break;
            }
        }
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
                ft_putstr_fd("exit\n", STDERR_FILENO);
                ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
                ft_putstr_fd(argv[1], STDERR_FILENO);
                ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
                exit(2);
            }
        i++;
    }
    if (argv[1] && argv[2])
    {
        ft_putstr_fd("exit\n", STDERR_FILENO);
        ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
        return (EXIT_FAILURE);
    }
    if (argv[1])
        exit_code = ft_atoi(argv[1]);
    else
        exit_code = 0;
    exit (exit_code & 0xFF);
}

int handle_builtins(t_tree *root, int in, int out, char ***env, char ***exported, int status)
{
    int exit_code;

    exit_code = status;
    if (!ft_strcmp(root->command[0], "echo"))
        exit_code = ft_echo(root->command, env, status);
    else if (!ft_strcmp(root->command[0], "cd"))
        exit_code = ft_cd(root->command, env);
    else if (!ft_strcmp(root->command[0], "pwd"))
        exit_code = ft_pwd(root->command);
    else if (!ft_strcmp(root->command[0], "export"))
        exit_code = ft_export(root->command,env, exported);
    else if (!ft_strcmp(root->command[0], "unset"))
        exit_code = ft_unset(root->command, env, exported);
    else if (!ft_strcmp(root->command[0], "env"))
        exit_code = ft_env(root->command, env[0]);
    else if (!ft_strcmp(root->command[0], "exit"))
        exit_code = ft_exit(root->command);
    return (exit_code);
}