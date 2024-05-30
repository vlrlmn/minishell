/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:52:25 by vlomakin          #+#    #+#             */
/*   Updated: 2024/05/30 17:36:48 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void handle_special_tokens(char **s, int *token) 
{
    if (*token == '\0')
        *token = '\0';
    else if (*token == '|')
        (*s)++;
    else if (*token == '>')
    {
        (*s)++;
        if (**s == '>') 
        {
            *token = '+';
            (*s)++;
        }
    } 
    else if (*token == '<') 
    {
        (*s)++;
        if (**s == '<') 
        {
            *token = '-';
            (*s)++;
        }
    } 
    else
        *token = 'a';
}

void skip_until_special_or_whitespace(char **s, char *es) 
{
    while (*s < es && !is_delimiter(**s) && !ft_strchr("<|>", **s)) 
    {
        if (**s == '\"') 
        {
            (*s)++;
            while (*s < es && **s != '\"')
                (*s)++;
        } 
        else if (**s == '\'') 
        {
            (*s)++;
            while (*s < es && **s != '\'')
                (*s)++;
        }
        if (*s < es)
            (*s)++;
    }
}

int gettoken(char **ps, char *es, char **q, char **eq)
{
    while (*ps < es && (**ps == ' ' || **ps == '\t' || **ps == '\n'))
        (*ps)++;
    if (*ps >= es)
        return 0;
    *q = *ps;
    if (**ps == '|' || **ps == '<' || **ps == '>')
    {
        (*ps)++;
        *eq = *ps;
        return **q;
    }
    while (*ps < es && (**ps != ' ' && **ps != '\t' && **ps != '\n'))
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
