/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aajaanan <aajaanan@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 17:32:22 by aajaanan          #+#    #+#             */
/*   Updated: 2023/09/01 20:10:07 by aajaanan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parsing.h"

int	check_invalid_pipe_syntax(char **ps, char *es, int *exit_status)
{
	if (peek(ps, es, "|"))
	{
		ft_printf_fd(STDERR_FILENO,
			"minishell: syntax error near unexpected token `|'\n");
		*exit_status = 258;
		return (1);
	}
	return (0);
}

int	validate_redirection(char **ps, char *es, int *exit_status)
{
	char	*q;
	char	*eq;
	int		tok;

	tok = get_next_token(ps, es, &q, &eq);
	if (tok == '\0')
	{
		ft_printf_fd(STDERR_FILENO,
			"minishell: syntax error near unexpected token `newline'\n");
		*exit_status = 258;
		return (0);
	}
	else if (tok != 'a')
	{
		*eq = '\0';
		ft_printf_fd(STDERR_FILENO,
			"minishell: syntax error near unexpected token `%s'\n", q);
		*exit_status = 258;
		return (0);
	}
	return (1);
}

int	validate_pipe(char **ps, char *es, int *exit_status)
{
	int	tok;

	tok = get_next_token(ps, es, 0, 0);
	if (tok == '|' || tok == '\0')
	{
		ft_printf_fd(STDERR_FILENO,
			"minishell: syntax error near unexpected token `|'\n");
		*exit_status = 258;
		return (0);
	}
	return (1);
}
