/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 15:34:37 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/14 12:21:28 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	validate_redirection(char **ps, char *es, int *exit_status)
{
	char	*q;
	char	*eq;
	int		tok;

	tok = get_token(ps, es, &q, &eq);
	if (tok == '\0')
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		*exit_status = 258;
		return (0);
	}
	// else if (tok != 'a')
	// {
	// 	*eq = '\0';
	// 	printf("minishell: syntax error near unexpected token \n");
	// 	exit_status = 258;
	// 	return (0);
	// }
	return (1);
}

int	check_invalid_pipe_syntax(char **ps, char *es, int *exit_status)
{
	if (peek(ps, es, "|"))
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		*exit_status = 258;
		return (1);
	}
	return (0);
}

int	validate_pipe(char **ps, char *es, int *exit_status)
{
	int	tok;

	tok = get_token(ps, es, 0, 0);
	if (tok == '|' || tok == '\0')
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		*exit_status = 258;
		return (0);
	}
	return (1);
}