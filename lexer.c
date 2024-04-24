/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:44:11 by vlomakin          #+#    #+#             */
/*   Updated: 2024/04/24 17:39:34 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char c)
{
	return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

int	quotes_balance(char *input)
{
	int	double_q;
	int	single_q;
	int	i;

	double_q = 0;
	single_q = 0;
	i = -1;
	while (input[i++])
	{
		if (input[i] == 39)
			single_q++;
		if (input[i] == 34)
			double_q++;
	}
	// printf("single quote: %d\n", single_q);
	// printf("double quote: %d\n", double_q);
	if (single_q % 2 != 0 || double_q % 2 != 0)
		return (QUOTES_ERR);
	else if (single_q && double_q)
		return (MULTIPLE_QUOTES);
	else if (single_q && !double_q)
		return (SINGLE_Q);
	else if (!single_q && double_q)
		return (DOUBLE_Q);
	else
		return (NO_QUOTES);
}

int	parenthesis_balance(char *input)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (input[i])
	{
		if (input[i] == '(')
			count++;
		else if (input[i] == ')')
		{
			if (count == 0)
				return (-1);
			count--;
		}
		i++;
	}
	if (count != 0)
		return (0);
	else
		return (1);
}

void	add_spaces(char *work_line, char *input, int input_len)
{
	int	i;
	int	j;
	int	double_q;
	int	single_q;

	i = 0;
	j = 0;
	double_q = 0;
	single_q = 0;
	while (i < input_len)
	{
		if (input[i] == '\'' && !double_q)
			single_q = !single_q;
		if (input[i] == '\"' && !single_q)
			double_q = !double_q;
		if (!single_q && !double_q && (input[i] == '>' || input[i] == '<'
				|| input[i] == '|' || input[i] == '$'))
		{
			if (j > 0 && work_line[j - 1] != ' ')
				work_line[j++] = ' ';
			if (input[i] == '>' && input[i + 1] == '>')
				work_line[j++] = input[++i];
			work_line[j++] = ' ';
		}
		else
			work_line[j++] = input[i];
		i++;
	}
	work_line[j] = '\0';
}

void	space_normalizer(char *line)
{
	int		i;
	int		j;
	char	last;
	int		in_quote;

	i = 0;
	j = 0;
	in_quote = 0;
	last = 0;
	while (line[i])
	{
		if (line[i] == 34 || line[i] == 39)
		{
			if (in_quote == line[i])
				in_quote = 0;
			else if (!in_quote)
				in_quote = line[i];
		}
		if (!in_quote && is_space(line[i]) && (last == ' ' || (last == 0 && i == 0)))
			;
		else
			line[j++] = line[i];
		last = line[i];
		i++;
	}
	if (last == ' ')
		j--;
	line[j] = '\0';
}

void	lexer(char *input)
{
	char *work_line;
	int len;

	len = ft_strlen(input);
	if (quotes_balance(input) == QUOTES_ERR || !parenthesis_balance(input))
		exit_with_syntax_err(SYNTAX_ERR); // change: ask to close the quote
			//EX_UNAVAILABLE is a code (2) for syntax err
	work_line = malloc(sizeof(char) * (len * 4));
	if (!work_line)
		exit_with_malloc_error(EX_UNAVAILABLE);
			//EX_UNAVAILABLE is a code (69) for error malloc
	add_spaces(work_line, input, len);
	space_normalizer(work_line);
	tokenize_input(work_line);
}
