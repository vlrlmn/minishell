/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlomakin <vlomakin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:52:25 by vlomakin          #+#    #+#             */
/*   Updated: 2024/05/15 12:26:33 by vlomakin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_word(char **s, char *es)
{
	while (*s < es && !is_delimiter(*s) && !ft_strchr("|<>", *s))
	{
		//PROCESS QUOTES//
		*s++;
	}
}

void	process_operators(char **s, char *es, int *ret)
{
	if (*s == '|')
		s++;
	else if (*s == '<')
	{
		s++;
		if (*s == '<')
		{
			*ret = '-';
			s++;
		}
	}
	else if (*s == '>')
	{
		s++;
		if (*s == '>')
		{
			*ret = '+';
			s++;
		}
	}
	else
		*ret = 'a';
}

int	gettoken(char **ps, char *es, char **q, char **eq)
{
	char	*s;
	int		ret;

	s = *ps;
	while (s < es && is_delimiter(*s))
		s++;
	if (q)
		*q = s;
	ret = *s;
	process_operators(&s, es, &ret);
	if (ret == 'a')
		process_word(&s, es);
	if (eq)
		*eq = s;
	while (s < es && is_delimiter(*s))
		s++;
	*ps = s;
	return (ret);
}

int	peek(char **ps, char **es, char *toks)
{
	char	*s;

	s = *ps;
	while (s < es && is_delimiter(*s))
		s++;
	*ps = s;
	return (*s && ft_strchr(toks, *s));
}
