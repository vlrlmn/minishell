/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/07/03 15:22:52 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

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
	{
		g_exit_status = 258;
		return (0);
	}
	else
	{
		return (1);
	}
}

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
			*token = '%';
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

int	validate_redirection(char **ps, char *es)
{
	char	*q;
	char	*eq;
	int		tok;

	tok = get_token(ps, es, &q, &eq);
	if (tok == '\0')
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		g_exit_status = 258;
		return (0);
	}
	else if (tok != 'a')
	{
		*eq = '\0';
		printf("minishell: syntax error near unexpected token '\n");
		g_exit_status = 258;
		return (0);
	}
	return (1);
}

int	check_invalid_pipe_syntax(char **ps, char *es)
{
	if (peek(ps, es, "|"))
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		g_exit_status = 258;
		return (1);
	}
	return (0);
}
int	validate_pipe(char **ps, char *es)
{
	int	tok;

	tok = get_token(ps, es, 0, 0);
	if (tok == '|' || tok == '\0')
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		g_exit_status = 258;
		return (0);
	}
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
		if (tok == '>' || tok == '<' || tok == '+' || tok == '%')
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
	while (is_delimiter(work_line[i]))
		i++;
	if (!work_line[i])
		return (0);
	if (!valid_quotes(work_line))
	{
		printf("Close quotes\n"); //TO_DO WRITE EXIT FROM  
		return (0);
	}
	if (!valid_syntax(work_line))
		return (0);
	return (1);
}
