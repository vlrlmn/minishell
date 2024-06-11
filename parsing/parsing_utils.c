/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:52:25 by vlomakin          #+#    #+#             */
/*   Updated: 2024/06/08 12:52:36 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int gettoken(char **ps, char *es, char **q, char **eq)
{
    int token;
    while (*ps < es && (**ps == ' ' || **ps == '\t' || **ps == '\n'))
        (*ps)++;
    if (*ps >= es)
        return 0;
    token = **ps;
    if (**ps == '|')
    {
        (*ps)++;
        *eq = *ps;
        return token;
    }
    else if (**ps == '<' || **ps == '>')
    {
        (*ps)++;
        if (**ps == '<')
        {
            (*ps)++;
            token = '+';
            return (token);
        }
        else if (**ps == '>')
        {
            (*ps)++;
            token = '-';
            return (token);
        }
        while(is_delimiter(**ps))
            (*ps)++;
        *q = *ps;
        while(!is_delimiter(**ps) && !ft_strchr("|<>", **ps))
            (*ps)++;
        *eq = *ps;
        return token;
    }
    if (**ps == '"')
    {
        (*ps)++;
        *q = *ps;
        while(*ps < es && **ps != '"')
            (*ps)++;
        if (*ps >= es)
            return (0);
        *eq = *ps;
        (*ps)++;
        return ('a');
    }
    else if (**ps == '\'')
    {
        (*ps)++;
        *q = *ps;
        while(*ps < es && **ps != '\'')
            (*ps)++;
        if (*ps >= es)
            return (0);
        *eq = *ps;
        (*ps)++;
        return ('a');
    }
    *q = *ps;
    while (*ps < es && (**ps != ' ' && **ps != '\t' && **ps != '\n') && !ft_strchr("<>|", **ps))
        (*ps)++;
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
