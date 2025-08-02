/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhafidi <rhafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 17:20:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/08/02 17:10:14 by rhafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_regular_char(char *result, char *str, int *i)
{
	char	*tmp;
	char	curr[2];

	curr[0] = str[*i];
	curr[1] = '\0';
	tmp = ft_strjoin(result, curr);
	free(result);
	(*i)++;
	return (tmp);
}

char	*process_character(char *result, char *str, int *i,
		t_expand_context *ctx)
{
	t_dollar_context	dctx;

	if (str[*i] == '\'' && !*ctx->in_double_quote)
	{
		*ctx->in_single_quote = !*ctx->in_single_quote;
		return (handle_single_quote(result, str, i, ctx->heredoc));
	}
	if (str[*i] == '"' && !*ctx->in_single_quote)
	{
		*ctx->in_double_quote = !*ctx->in_double_quote;
		return (handle_double_quote(result, str, i, ctx->heredoc));
	}
	if (str[*i] == '$' && (ctx->heredoc == 1 || !*ctx->in_single_quote))
	{
		dctx.env = ctx->env;
		dctx.status = ctx->status;
		dctx.doll = ctx->doll;
		return (handle_dollar_cases(result, str, i, &dctx));
	}
	return (handle_regular_char(result, str, i));
}

char	*expand_string(char *str, char **env, int status, int heredoc)
{
	t_expand_context	ctx;
	int					i;
	int					in_single_quote;
	int					in_double_quote;
	int					doll;

	if (!str)
		return (NULL);
	ctx.result = ft_strdup("");
	if (!ctx.result)
		return (NULL);
	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	doll = 0;
	ctx.env = env;
	ctx.status = status;
	ctx.doll = &doll;
	ctx.in_single_quote = &in_single_quote;
	ctx.in_double_quote = &in_double_quote;
	ctx.heredoc = heredoc;
	ctx.i = &i;
	while (str[i])
		ctx.result = process_character(ctx.result, str, &i, &ctx);
	return (ctx.result);
}

char	**expand(char **argv, char **env, int status)
{
	int		i;
	int		len;
	char	**expanded;

	if (!argv || !argv[0])
		return (NULL);
	len = 0;
	while (argv[len])
		len++;
	expanded = malloc(sizeof(char *) * (len + 1));
	if (!expanded)
		return (NULL);
	i = 0;
	while (i < len)
	{
		expanded[i] = expand_string(argv[i], env, status, 1);
		i++;
	}
	expanded[i] = NULL;
	return (expanded);
}
