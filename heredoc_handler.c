/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 17:42:52 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 19:20:32 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_heredoc_line(char *line, char *delimiter, int pipefd, char **env)
{
	char	**expanded;

	if (ft_strchr(delimiter, '\'') || ft_strchr(delimiter, '\"'))
	{
		write(pipefd, line, ft_strlen(line));
		write(pipefd, "\n", 1);
	}
	else
	{
		expanded = expand(&line, env, 0);
		write(pipefd, expanded[0], ft_strlen(expanded[0]));
		write(pipefd, "\n", 1);
		free_array(expanded);
	}
}

int	heredoc_loop(char *tmp_delimiter, int pipefd, char **env, char *delimiter)
{
	char	*line[2];

	line[1] = NULL;
	while (1)
	{
		line[0] = readline("> ");
		if (!line[0])
		{
			write(STDOUT_FILENO, "\n", 1);
			break ;
		}
		if (!ft_strcmp(line[0], tmp_delimiter))
			break ;
		write_heredoc_line(line[0], delimiter, pipefd, env);
		free(line[0]);
	}
	free(line[0]);
	return (0);
}

int	handle_heredoc(char *delimiter, char **env)
{
	int		pipefd[2];
	char	*tmp_delimiter[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe failed");
		return (-1);
	}
	signal(SIGINT, heredoc_sigint_handler);
	tmp_delimiter[1] = NULL;
	tmp_delimiter[0] = remove_quotes_from_string(delimiter, 1);
	heredoc_loop(tmp_delimiter[0], pipefd[1], env, delimiter);
	close(pipefd[1]);
	signal(SIGINT, sigint_handler);
	free(tmp_delimiter[0]);
	return (pipefd[0]);
}
