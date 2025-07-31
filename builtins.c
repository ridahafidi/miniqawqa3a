/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 17:21:18 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/31 18:38:45 by rhafidi          ###   ########.fr       */
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

void    print_args(char **argv, int i)
{
    while (argv[i])
    {
        ft_putstr_fd(argv[i], STDOUT_FILENO);
        i++;
        if(argv[i])
           write(STDOUT_FILENO, " ", 1); 
    }
}

int no_args()
{
    write(STDOUT_FILENO, "\n", 1);
    return (0);
}
int ft_echo(char **argv, char ***env, int *exit_status)
{
    int i;
    int n_flag;
    
    i = 1;
    n_flag = 0;
    if (!argv[i])
        return(no_args());
    while (argv[i] && argv[i][0] == '-')
    {
        int j = 1;
        while (argv[i][j] == 'n')
            j++;
        if (argv[i][j] != '\0')
            break;
        if (j == 1)
            break;
        n_flag = 1;
        i++;
    }
    print_args(argv, i);
    if (n_flag == 0)
        write(STDOUT_FILENO, "\n", 1);
    return (0);
}

// cd

char    *ft_getenv(char **env, const char *name)
{
    int     i;
    size_t  len;

    if (!env || !name)
        return (NULL);
    len = ft_strlen(name);
    i = 0;
    while (env[i])
    {
        if (!ft_strncmp(env[i], name, len) && env[i][len] == '=')
            return (env[i] + len + 1); // return value after '='
        i++;
    }
    return (NULL);
}

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
/*/cd boundries*/

static int	count_arguments(char **argv)
{
	int	arg_count;

	arg_count = 0;
	while (argv[arg_count])
		arg_count++;
	return (arg_count);
}

static int	change_to_home(char ***env, char *cwd)
{
	char			*path;
	struct stat		file_stat;

	path = ft_getenv(*env ,"HOME");
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
		return (1);
	}
	return (0);
}

static int	change_to_oldpwd(char ***env, char *cwd)
{
	char			*path;
	struct stat		file_stat;

	path = ft_getenv(*env, "OLDPWD");
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
		ft_putstr_fd(path, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	else
	{
		perror("not dir or permession denied");
		return (1);
	}
	return (0);
}

static int	change_to_path(char *path)
{
	struct stat	file_stat;

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
		return (1);
	}
	return (0);
}

static int	handle_directory_change(char *path, char ***env, char *cwd)
{
	if (!path)
		return (change_to_home(env, cwd));
	else if (!ft_strcmp(path, "-"))
		return (change_to_oldpwd(env, cwd));
	else
		return (change_to_path(path));
}

static int	validate_and_get_path(char **argv, char **path)
{
	int	arg_count;

	arg_count = count_arguments(argv);
	if (arg_count > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (EXIT_FAILURE);
	}
	if (arg_count == 2)
		*path = argv[1];
	else
		*path = NULL;
	return (0);
}

static char *get_current_pwd_from_env(int *should_free, char ***env)
{
    char *current_pwd;
    char *tmp;

    tmp = ft_getenv(*env, "PWD");
    *should_free = 0;
    if (tmp)
    {
        current_pwd = ft_strdup(tmp); // duplicate because update_env() will free PWD
        if (!current_pwd)
            return (NULL);
        *should_free = 1;
        return (current_pwd);
    }
    current_pwd = malloc(4096);
    *should_free = 1;
    if (!current_pwd || getcwd(current_pwd, 4096) == NULL)
    {
        if (current_pwd)
            free(current_pwd);
        return (NULL);
    }
    return (current_pwd);
}

static int	handle_cd_operation(char *path, char ***env, char *current_pwd)
{
	char	new_cwd[4096];

	if (handle_directory_change(path, env, current_pwd) != 0)
		return (1);
	if (getcwd(new_cwd, sizeof(new_cwd)) == NULL)
	{
		perror("getcwd failed");
		return (1);
	}
	update_env(env, new_cwd, current_pwd);
	return (0);
}

int	ft_cd(char **argv, char ***env)
{
	char	*path;
	char	*current_pwd;
	int		should_free_pwd;
	int		result;

	if (validate_and_get_path(argv, &path) != 0)
		return (EXIT_FAILURE);
	current_pwd = get_current_pwd_from_env(&should_free_pwd, env);
	if (!current_pwd)
	{
		perror("getcwd failed");
		return (1);
	}
	result = handle_cd_operation(path, env, current_pwd);
	if (should_free_pwd)
		free(current_pwd);
	return (result);
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
    int equal_pos;

    i = 0;
    equal_pos = -1;
    if (!str || !*str)
        return (0);
    
    // Find the position of '=' or '+=' if it exists
    while (str[i] && str[i] != '=')
    {
        if (str[i] == '+' && str[i + 1] == '=')
        {
            equal_pos = i;  // Position of '+' in '+='
            break;
        }
        i++;
    }
    if (str[i] == '=' && equal_pos == -1)
        equal_pos = i;
    
    // Check the variable name part (before '=' or '+=' or entire string if neither exists)
    i = 0;
    
    // First character must be a letter or underscore
    if (!ft_isalpha(str[i]) && str[i] != '_')
        return (0);
    i++;
    
    // Subsequent characters must be letters, digits, or underscores
    // Stop at '+=' or '=' whichever comes first
    while (str[i] && (equal_pos == -1 || i < equal_pos))
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
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
    if (!variable_name)
        return (NULL);
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
    // tmp = value;
    quote = "\"";
    tmp = ft_strjoin(quote, value);
    free(value);
    value = tmp;
    tmp = ft_strjoin(value, quote);
    free(value);
    value = tmp;
    variable_name = copy_var(export, i);
    free(export[i]);
    export[i] = ft_strjoin(variable_name, value);
    free(variable_name);
    free(value);
}

void    print_format(char **export)
{
    int i;
    int j;
    char    *tmp;
    char    *declare;

    i = 0;
    j = 0;
    declare = "declare -x ";
    while (export[i])
    {
        tmp = ft_strjoin(declare, export[i]);
        free(export[i]);
        export[i] = tmp; 
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

// Helper: check if var is in exported//////////////////////////////////////////////////////////////////
int	is_in_exported(char *var, char **exported)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (var[len] && var[len] != '=' && var[len] != '+')
		len++;
	if (var[len] == '+' && var[len + 1] == '=')
		return (-2);
	while (exported && exported[i])
	{
		if (!ft_strncmp(exported[i], var, len) &&
			(exported[i][len] == '\0' || exported[i][len] == '='))
			return (i);
		i++;
	}
	return (-1);
}

static char	*create_appended_var(char *old_var, char *append_value, int var_name_len)
{
	char	*new_var;
	char	*temp;

	if (old_var[var_name_len] == '=')
		new_var = ft_strjoin(old_var, append_value);
	else
	{
		temp = ft_strjoin(old_var, "=");
		new_var = ft_strjoin(temp, append_value);
		free(temp);
	}
	return (new_var);
}

static int	handle_append_exported(char *var, char ***exported, int len)
{
	int		i;
	int		var_name_len;
	char	*old_var;
	char	*append_value;

	var_name_len = 0;
	while (var[var_name_len] && var[var_name_len] != '+')
		var_name_len++;
	append_value = var + var_name_len + 2;
	i = 0;
	while (i < len)
	{
		if (!ft_strncmp((*exported)[i], var, var_name_len) && 
			((*exported)[i][var_name_len] == '=' || 
			(*exported)[i][var_name_len] == '\0'))
		{
			old_var = (*exported)[i];
			(*exported)[i] = create_appended_var(old_var, append_value, var_name_len);
			free(old_var);
			return (1);
		}
		i++;
	}
	return (0);
}

void    copy_to_new_exported(char **new_exported, char ***exported, int len, int i)
{
    while (i < len)
	{
		new_exported[i] = (*exported)[i];
		i++;
	}
}

static void	create_new_append_var(char *var, char ***exported, int len)
{
	int		i;
	int		var_name_len;
	char	*var_name;
	char	*temp;
	char	*new_var;
	char	**new_exported;

	var_name_len = 0;
	while (var[var_name_len] && var[var_name_len] != '+')
		var_name_len++;
	var_name = ft_substr(var, 0, var_name_len);
	temp = ft_strjoin(var_name, "=");
	new_var = ft_strjoin(temp, var + var_name_len + 2);
	free(var_name);
	free(temp);
	new_exported = malloc(sizeof(char *) * (len + 2));
	i = 0;
    copy_to_new_exported(new_exported, exported, len, i);
	new_exported[len] = new_var;
	new_exported[len + 1] = NULL;
	if (*exported)
		free(*exported);
	*exported = new_exported;
}

static void	add_new_exported(char *var, char ***exported, int len)
{
	char	**new_exported;
	int		i;

	new_exported = malloc(sizeof(char *) * (len + 2));
	i = 0;
	while (i < len)
	{
		new_exported[i] = (*exported)[i];
		i++;
	}
	new_exported[len] = ft_strdup(var);
	new_exported[len + 1] = NULL;
	if (*exported)
		free(*exported);
	*exported = new_exported;
}

void	add_or_update_exported(char *var, char ***exported)
{
	int	idx;
	int	len;

	idx = is_in_exported(var, *exported);
	len = 0;
	while (*exported && (*exported)[len])
		len++;
	if (idx >= 0)
	{
		free((*exported)[idx]);
		(*exported)[idx] = ft_strdup(var);
	}
	else if (idx == -2)
	{
		if (!handle_append_exported(var, exported, len))
			create_new_append_var(var, exported, len);
	}
	else
		add_new_exported(var, exported, len);
}

static void	create_new_append_env(char *var, char ***env, int len, int var_name_len)
{
    char	*var_name;
    char	*temp;
    char	*new_var;
    char	**new_env;
    int		i;

    var_name = ft_substr(var, 0, var_name_len);
    temp = ft_strjoin(var_name, "=");
    new_var = ft_strjoin(temp, var + var_name_len + 2);
    free(var_name);
    free(temp);
    new_env = malloc(sizeof(char *) * (len + 2));
    i = 0;
    while (i < len)
    {
        new_env[i] = (*env)[i];
        i++;
    }
    new_env[len] = new_var;
    new_env[len + 1] = NULL;
    if (*env)
        free(*env);
    *env = new_env;
}
static void	handle_append_env(char *var, char ***env, int len, int var_name_len)
{
	int		i;
	char	*old_var;
	char	*append_value;

	append_value = var + var_name_len + 2;
	i = 0;
	while (i < len)
	{
		if (!ft_strncmp((*env)[i], var, var_name_len) && 
			(*env)[i][var_name_len] == '=')
		{
			old_var = (*env)[i];
			(*env)[i] = ft_strjoin(old_var, append_value);
			free(old_var);
			return ;
		}
		i++;
	}
	create_new_append_env(var, env, len, var_name_len);
}


static int	check_append_operation(char *var)
{
	int	var_name_len;

	var_name_len = 0;
	while (var[var_name_len])
	{
		if (var[var_name_len] == '+' && var[var_name_len + 1] == '=')
			return (var_name_len);
		if (var[var_name_len] == '=')
			break ;
		var_name_len++;
	}
	return (-1);
}
static int get_eqpos(char *var)
{
    int eqpos;

    eqpos = 0;
    while (var[eqpos] && var[eqpos] != '=')
		eqpos++;
    return (eqpos);
}

static void	handle_regular_assignment(char *var, char ***env, int len)
{
	int		eqpos;
	int		i;
	char	**new_env;

	eqpos = get_eqpos(var);
	i = 0;
	while (i < len)
	{
		if (!ft_strncmp((*env)[i], var, eqpos) && (*env)[i][eqpos] == '=')
		{
			free((*env)[i]);
			(*env)[i] = ft_strdup(var);
			return ;
		}
		i++;
	}
	new_env = malloc(sizeof(char *) * (len + 2));
	i = -1;
	while (++i < len)
		new_env[i] = (*env)[i];
	new_env[len] = ft_strdup(var);
	new_env[len + 1] = NULL;
	if (*env)
		free(*env);
	*env = new_env;
}

void	add_or_update_env(char *var, char ***env)
{
	int	len;
	int	var_name_len;

	if (!ft_strchr(var, '='))
		return ;
	len = 0;
	while (*env && (*env)[len])
		len++;
	var_name_len = check_append_operation(var);
	if (var_name_len >= 0)
		handle_append_env(var, env, len, var_name_len);
	else
		handle_regular_assignment(var, env, len);
}

void	add_var(char **argv, char ***env, char ***exported, int *exit_status)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
			*exit_status = 1;
		}
		else
		{
			if (ft_strchr(argv[i], '='))
			{
				add_or_update_exported(argv[i], exported);
				add_or_update_env(argv[i], env);
			}
			else
				add_or_update_exported(argv[i], exported);
			*exit_status = 0;
		}
		i++;
	}
}

static void	print_exported_vars(char **copy)
{
	int	i;

	i = 0;
	while (copy[i])
	{
		ft_putstr_fd(copy[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
}

static void	free_copy_array(char **copy)
{
	int	i;

	i = 0;
	while (copy[i])
	{
		free(copy[i]);
		i++;
	}
	free(copy);
}

int	ft_export(char **argv, char ***env, char ***exported, int *exit_status)
{
	char	**copy;

	if (!argv[1])
	{
		copy = copy_env(*exported);
		sort_export(copy);
		print_format(copy);
		print_exported_vars(copy);
		free_copy_array(copy);
		return (EXIT_SUCCESS);
	}
	else
	{
		add_var(argv, env, exported, exit_status);
		return (*exit_status);
	}
	return (EXIT_FAILURE);
}
// env ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

static void	remove_from_env(char *arg, char ***env)
{
	int		len;
	int		j;
	int		n;

	len = 0;
	while ((*env)[len])
		len++;
	j = 0;
	while (j < len)
	{
		n = ft_strlen(arg);
		if (!ft_strncmp(arg, (*env)[j], n)
			&& ((*env)[j][n] == '=' || (*env)[j][n] == '\0'))
		{
			ft_swap_ptr(env, j, len - 1);
			free((*env)[len - 1]);
			(*env)[len - 1] = NULL;
			break ;
		}
		j++;
	}
}

int	ft_unset(char **argv, char ***env, char ***exported)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		remove_from_env(argv[i], env);
		remove_from_env(argv[i], exported);
		i++;
	}
	return (EXIT_SUCCESS);
}

void    free_mem(t_tree *root, char **env, char **exported, t_fd *fd)
{
    free_tree(&root);
    free(fd);
    free_array(env);
    free_array(exported);
}

void    exit_error(char **argv)
{
    ft_putstr_fd("exit\n", STDERR_FILENO);
    ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
    ft_putstr_fd(argv[1], STDERR_FILENO);
    ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
}

int    many_args()
{
    ft_putstr_fd("exit\n", STDERR_FILENO);
    ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
    return (EXIT_FAILURE);
}

int ret_ex_code(char **argv)
{
    int exit_code;

    if (argv[1])
        exit_code = ft_atoi(argv[1]);
    else
        exit_code = 0;
    return(exit_code);
}
void    clean_exit(t_tree *root, char **env, char **exported, t_fd *fd, int exit_status)
{
        free_mem(root, env, exported, fd);
        exit(exit_status);    
}
int    ft_exit(t_tree *root, char **env, char **exported, t_fd *fd, int *exit_status)
{
    int i;
    int exit_code;
    char **argv;
    
    argv = root->command;
    i = 0;
    clear_history();
    if (!argv[1])
        clean_exit(root, env, exported, fd, *exit_status);
    while (argv[1][i])
    {
        if ((argv[1][i] < '0' || argv[1][i] > '9') && argv[1][i] != '+' && argv[1][i] != '-')
            {
                exit_error(argv);
                free_mem(root, env, exported, fd);
                exit(2);
            }
        i++;
    }
    if (argv[1] && argv[2])
        return (many_args());
    exit_code = ret_ex_code(argv);
    free_mem(root, env, exported, fd);
    exit (exit_code & 0xFF);
}

int handle_builtins(t_tree *root, t_fd *fd, char ***env, char ***exported, int *exit_status)
{
    int exit_code;

    exit_code = *exit_status;
    if (!ft_strcmp(root->command[0], "echo"))
        exit_code = ft_echo(root->command, env, exit_status);
    else if (!ft_strcmp(root->command[0], "cd"))
        exit_code = ft_cd(root->command, env);
    else if (!ft_strcmp(root->command[0], "pwd"))
        exit_code = ft_pwd(root->command);
    else if (!ft_strcmp(root->command[0], "export"))
        exit_code = ft_export(root->command,env, exported, exit_status);
    else if (!ft_strcmp(root->command[0], "unset"))
        exit_code = ft_unset(root->command, env, exported);
    else if (!ft_strcmp(root->command[0], "env"))
        exit_code = ft_env(root->command, env[0]);
    else if (!ft_strcmp(root->command[0], "exit"))
        exit_code = ft_exit(root, *env, *exported, fd, exit_status);
    return (exit_code);
    
}
