/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:52:25 by vlomakin          #+#    #+#             */
/*   Updated: 2024/07/07 17:28:29 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int handle_pipe(char **ps, char **eq)
{
    (*ps)++;
    *eq = *ps;
    return '|';
}

int handle_redirect(char **ps, char *es, char **q, char **eq)
{
    int token;

    if (**ps == '<')
        token = '+';
    else
        token = '-';
    (*ps)++;
    skip_delimiters(ps, es);
    *q = *ps;
    while (*ps < es && !is_delimiter(**ps) && !ft_strchr("|<>", **ps))
        (*ps)++;
    *eq = *ps;
    return token;
}

int handle_word(char **ps, char *es, char **q, char **eq)
{
    *q = *ps;
    while (*ps < es && !is_delimiter(**ps) && !ft_strchr("<>|", **ps))
    {
        if (**ps == '"' || **ps == '\'')
            handle_quotes(ps, es);
        else
            (*ps)++;
    }
    *eq = *ps;
    return 'a';
}

int gettoken(char **ps, char *es, char **q, char **eq)
{
    int token;

    skip_delimiters(ps, es);
    if (*ps >= es)
        return 0;
    token = **ps;
    if (token == '|')
        return handle_pipe(ps, eq);
    else if (token == '<' || token == '>')
        return handle_redirect(ps, es, q, eq);
    else
        return handle_word(ps, es, q, eq);
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
