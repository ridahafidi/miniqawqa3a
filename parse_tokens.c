/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:52:23 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/11 15:12:26 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void strip_quotes_from_ast(t_tree *ast)
{
    int i;
    char *new_str;
    
    if (!ast)
        return;
    
    // If this is a command node, strip quotes from its arguments
    if (ast->type == COMMAND && ast->command)
    {
        i = 0;
        while (ast->command[i])
        {
            new_str = remove_quotes_from_string(ast->command[i]);
            if (new_str)
            {
                free(ast->command[i]);
                ast->command[i] = new_str;
            }
            i++;
        }
    }
    
    // For heredoc nodes, DON'T strip quotes from the delimiter (file_name)
    // but still process the left child (command)
    if (ast->type == HEREDOC)
    {
        // Only process the left child (command), skip the delimiter
        if (ast->left)
            strip_quotes_from_ast(ast->left);
        // Don't process right child or file_name for heredoc
        return;
    }
    
    // For all other redirection types, process both children
    if (ast->left)
        strip_quotes_from_ast(ast->left);
    if (ast->right)
        strip_quotes_from_ast(ast->right);
}

t_tree	*create_command_node(char **cmd_args)
{
	t_tree	*cmd_node;

	cmd_node = create_command();
	if (!cmd_node)
		return (NULL);
	cmd_node->type = COMMAND;
	cmd_node->command = cmd_args;
	return (cmd_node);
}

t_tree	*process_redirections(t_tree *cmd_node, char **tokens, int start,
		int end)
{
	int		i;
	int		redir_type;
	t_tree	*root;
	t_tree	*redir_node;

	root = cmd_node;
	i = start;
	while (i <= end)
	{
		redir_type = is_redirections(tokens[i]);
		if (redir_type)
		{
			if (i + 1 <= end)
			{
				redir_node = create_redirections(tokens[i], tokens[i + 1]);
				redir_node->left = root;
				root = redir_node;
				i += 2;
			}
			else
				return (NULL);
		}
		else
			i++;
	}
	return (root);
}

int	initialize_end_index(char **tokens, int end)
{
	if (end == -1)
	{
		end = 0;
		while (tokens[end])
			end++;
		end--;
	}
	return (end);
}
char	**collect_command_arguments(char **tokens, int start, int end)
{
	char	**cmd_args;
	int		i;
	int		arg_count;

	cmd_args = malloc(sizeof(char *) * (end - start + 2));
	if (!cmd_args)
		return (NULL);
	arg_count = 0;
	i = start;
	while (i <= end)
    {
        if (is_redirections(tokens[i]))
        {
            i += 2;
            if (i > end + 1)
            {
                while (arg_count > 0)
                    free(cmd_args[--arg_count]);
                free(cmd_args);
                return NULL;
            }
            continue;
        }
        cmd_args[arg_count] = ft_strdup(tokens[i]);
        if (!cmd_args[arg_count])
        {
            // Cleanup if strdup fails
            while (arg_count > 0)
                free(cmd_args[--arg_count]);
            free(cmd_args);
            return NULL;
        }
        arg_count++;
        i++;
    }
    cmd_args[arg_count] = NULL;
    return cmd_args;
}


t_tree	*parse_token_subset(char **tokens, int start, int end)
{
	int		i;
	int		arg_count;
	t_tree *ast;
	t_tree	*cmd_node;
	t_tree	*root;
	t_tree	*redir_node;
	char	**cmd_args;

	end = initialize_end_index(tokens, end);
	if (start > end || !tokens[start])
		return (NULL);
	cmd_args = collect_command_arguments(tokens, start, end);
	if (!cmd_args)
		return (NULL);
	cmd_node = create_command_node(cmd_args);
	if (!cmd_node)
	{
		free (cmd_args);
		return (NULL);
	}
	ast = process_redirections(cmd_node, tokens, start, end);
	strip_quotes_from_ast(ast);
	// if (ast->type != HEREDOC && ast->command)
	// {
	// i = 0;
	// while (ast->command[i])
	// {
	// 	printf("here\n");
	// 	ast->command[i] = remove_quotes_from_string(ast->command[i]);
	// 	i++;
	// }
	// }
	return (ast);
}

t_tree	*create_pipe_node(void)
{
	t_tree	*cmd;

	cmd = create_command();
	if (!cmd)
		return (NULL);
	cmd->type = PIPE;
	return (cmd);
}

void	ft_print_tree(t_tree *root)
{
	if (root->type == PIPE)
	{
		ft_print_tree(root->left);
		ft_print_tree(root->right);
	}
	else if (root->type == GREATER)
		ft_print_tree(root->left);
	else
	{
		for (int i = 0; root->command[i] != NULL; i++)
			printf("%s\n", root->command[i]);
	}
}

t_tree	*parse_tokens(char **tokens)
{
	int		i;
	int		start;
	int		heredoc_flag;
	t_tree	*ast;
	t_tree	*pipe_node;
	t_tree	*cmd_node;
	t_tree	*redirection_node;

	start = 0;
	i = 0;
	while (tokens[i])
	{
		heredoc_flag = 0;
		if (ft_strcmp(tokens[i], "|") == 0)
		{
			pipe_node = create_pipe_node();
			pipe_node->left = parse_token_subset(tokens, start, i - 1);
			start = i + 1;
			pipe_node->right = parse_tokens(tokens + start);
			return (pipe_node);
		}
		i++;
	}
	return (parse_token_subset(tokens, 0, -1));
}
