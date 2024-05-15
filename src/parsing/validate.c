/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aajaanan <aajaanan@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 17:32:19 by aajaanan          #+#    #+#             */
/*   Updated: 2023/09/04 09:42:45 by aajaanan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parsing.h"

static int	validate_syntax(char *buf, int *exit_status)
{
	char	*ps;
	char	*es;
	int		tok;

	ps = buf;
	es = ps + ft_strlen(buf);
	if (check_invalid_pipe_syntax(&ps, es, exit_status))
		return (0);
	while (!peek(&ps, es, "\0"))
	{
		tok = get_next_token(&ps, es, 0, 0);
		if (tok == '>' || tok == '<' || tok == '+' || tok == '%')
		{
			if (!validate_redirection(&ps, es, exit_status))
				return (0);
		}
		else if (tok == '|')
		{
			if (!validate_pipe(&ps, es, exit_status))
				return (0);
		}
		else if (tok == '\0')
			break ;
	}
	return (1);
}

static int	contains_unbalanced_quotes(char *buf, int *exit_status)
{
	int	i;
	int	tok;

	i = 0;
	while (buf[i])
	{
		if (buf[i] == '\"' || buf[i] == '\'')
		{
			tok = buf[i];
			i++;
			while (buf[i] && buf[i] != tok)
				i++;
			if (!buf[i])
			{
				ft_printf_fd(STDERR_FILENO, "minishell: unexpected EOF while looking \
for matching `%c'\n", tok);
				*exit_status = 258;
				return (1);
			}
			i++;
		}
		else
			i++;
	}
	return (0);
}

int	validate_command(char *buf, int *exit_status)
{
	if (contains_unbalanced_quotes(buf, exit_status))
		return (0);
	if (!validate_syntax(buf, exit_status))
		return (0);
	return (1);
}
