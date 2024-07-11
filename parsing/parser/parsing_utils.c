/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:52:25 by vlomakin          #+#    #+#             */
/*   Updated: 2024/07/11 17:54:55 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
            while (is_delimiter(**ps))
                (*ps)++;
            *q = *ps;
            while (!is_delimiter(**ps) && !ft_strchr("|<>", **ps))
                (*ps)++;
            *eq = *ps;
            token = '+';
            return (token);
        }
        else if (**ps == '>')
        {
            (*ps)++;
            while (is_delimiter(**ps))
                (*ps)++;
            *q = *ps;
            while (!is_delimiter(**ps) && !ft_strchr("|<>", **ps))
                (*ps)++;
            *eq = *ps;
            token = '-';
            return (token);
        }
        while (is_delimiter(**ps))
            (*ps)++;
        *q = *ps;
        while (!is_delimiter(**ps) && !ft_strchr("|<>", **ps))
            (*ps)++;
        *eq = *ps;
        return token;
    }
    *q = *ps;
    while (*ps < es && !is_delimiter(**ps) && !ft_strchr("<>|", **ps))
    {
        if (**ps == '"')
        {
            (*ps)++;
            while(*ps < es && **ps != '"')
                (*ps)++;
            if (*ps >= es)
                return (0);
            (*ps)++;
        }
        else if (**ps == '\'')
        {
            (*ps)++;
            while(*ps < es && **ps != '\'')
                (*ps)++;
            if (*ps >= es)
                return (0);
            (*ps)++;  
        }
        else if (*ps < es)
            (*ps)++;
    }
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
