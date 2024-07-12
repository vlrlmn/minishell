/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/07/11 17:11:55 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	valid_quotes(char *input)
{
	int	count_single;
	int	count_double;
	int	i;

	i = 0;
	count_double = 0;
	count_single = 0;
	while (input[i])
	{
		if (input[i] == 34)
			count_double++;
		if (input[i] == 39)
			count_single++;
		i++;
	}
	if (count_single % 2 != 0 || count_double % 2 != 0)
		return (0);
	else
		return (1);
}

int valid_syntax(char *work_line)
{
	char	*ps;
	char	*es;
	int		tok;

	ps = work_line;
	es = ps + ft_strlen(work_line);
	if (check_invalid_pipe_syntax(&ps, es))
		return (0);
	while (!peek(&ps, es, "\0"))
	{
		tok = get_token(&ps, es, 0, 0);
		if (tok == '>' || tok == '<' || tok == '+' || tok == '-')
		{
			if (!validate_redirection(&ps, es))
				return (0);
		}
		else if (tok == '|')
		{
			if (!validate_pipe(&ps, es))
				return (0);
		}
		else if (tok == '\0')
			break ;
	}
	return (1);
}

int	valid_input(char *work_line)
{
	int	i;

	i = 0;
	if (ft_strlen(work_line) == 0)
		return(0);
	if (ft_strlen(work_line) == 2 && work_line[0] == '<' && work_line[1] == '>')
	{
		g_exit_status = 258;
		return (0);
	}
	while (is_delimiter(work_line[i]))
		i++;
	if (!work_line[i])
		return (0);
	if (!valid_quotes(work_line))
	{
		printf("Close quotes\n");
		return (0);
	}
	if (!valid_syntax(work_line))
		return (0);
	return (1);
}
