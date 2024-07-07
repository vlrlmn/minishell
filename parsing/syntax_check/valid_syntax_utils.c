/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_syntax_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 15:37:33 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/07 15:47:36 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	process_special_tokens(char **s, int *token)
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

static void	skip_non_special_tokens(char **s, char *es)
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

int	get_token(char **ps, char *es, char **q, char **eq)
{
	int		token;
	char	*s;

	s = *ps;
	while (s < es && is_delimiter(*s))
		s++;
	if (q)
		*q = s;
	token = *s;
	process_special_tokens(&s, &token);
	if (token == 'a')
		skip_non_special_tokens(&s, es);
	if (eq)
		*eq = s;
	while (s < es && is_delimiter(*s))
		s++;
	*ps = s;
	return (token);
}
