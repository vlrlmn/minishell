/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:52:25 by vlomakin          #+#    #+#             */
/*   Updated: 2024/05/27 18:43:05 by lomakinaval      ###   ########.fr       */
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
    int token;
    char *s = *ps;

    while (s < es && is_delimiter(*s))
        s++;
    if (q)
        *q = s;
    token = *s;
    handle_special_tokens(&s, &token);
    if (token == 'a')
        skip_until_special_or_whitespace(&s, es);
    if (eq)
        *eq = s;
    while (s < es && is_delimiter(*s))
        s++;
    *ps = s;
    return token;
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
