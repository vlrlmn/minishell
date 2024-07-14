/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlomakin <vlomakin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:52:25 by vlomakin          #+#    #+#             */
/*   Updated: 2024/07/14 16:33:15 by vlomakin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_redir_token(char **ps, char **q, char **eq, int *token)
{
	(*ps)++;
	if (**ps == '<')
		return (redir_tok_heredoc(ps, q, eq, token));
	else if (**ps == '>')
		return (redir_tok_append(ps, q, eq, token));
	while (is_delimiter(**ps))
		(*ps)++;
	*q = *ps;
	while (!is_delimiter(**ps) && !ft_strchr("|<>", **ps))
		(*ps)++;
	*eq = *ps;
	return (*token);
}

int	skip_characters(char **ps, char *es)
{
	while (*ps < es && !is_delimiter(**ps) && !ft_strchr("<>|", **ps))
	{
		if (**ps == '"')
		{
			(*ps)++;
			while (*ps < es && **ps != '"')
				(*ps)++;
			if (*ps >= es)
				return (0);
			(*ps)++;
		}
		else if (**ps == '\'')
		{
			(*ps)++;
			while (*ps < es && **ps != '\'')
				(*ps)++;
			if (*ps >= es)
				return (0);
			(*ps)++;
		}
		else if (*ps < es)
			(*ps)++;
	}
	return (1);
}

int	get_pipe_token(char **ps, char **eq, int *token)
{
	(*ps)++;
	*eq = *ps;
	return (*token);
}

int	gettoken(char **ps, char *es, char **q, char **eq)
{
	int	token;

	while (*ps < es && is_delimiter(**ps))
		(*ps)++;
	if (*ps >= es)
		return (0);
	token = **ps;
	if (**ps == '|')
		return (get_pipe_token(ps, eq, &token));
	else if (**ps == '<' || **ps == '>')
		return (get_redir_token(ps, q, eq, &token));
	*q = *ps;
	if (!skip_characters(ps, es))
		return (0);
	*eq = *ps;
	return ('a');
}

int	peek(char **ps, char *es, char *toks)
{
	char	*s;

	s = *ps;
	while (s < es && is_delimiter(*s))
		s++;
	*ps = s;
	return (*s && ft_strchr(toks, *s));
}
