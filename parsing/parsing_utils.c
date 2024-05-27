/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:52:25 by vlomakin          #+#    #+#             */
/*   Updated: 2024/05/27 15:32:09 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_operators(char **s, int *ret)
{
	if (**s == '|')
		s++;
	else if (**s == '<')
	{
		s++;
		if (**s == '<')
		{
			*ret = '-';
			s++;
		}
	}
	else if (**s == '>')
	{
		s++;
		if (**s == '>')
		{
			*ret = '+';
			s++;
		}
	}
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
	process_operators(&s, &ret);
	if (ret == 'a')
	{
		while (s < es && !is_delimiter(*s) && !ft_strchr("|<>", *s))
		{
			//PROCESS QUOTES//
			s++;
		}
	}
	if (eq)
		*eq = s;
	while (s < es && is_delimiter(*s))
		s++;
	*ps = s;
	return (ret);
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
