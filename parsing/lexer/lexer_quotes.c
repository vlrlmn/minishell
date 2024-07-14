/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlomakin <vlomakin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 15:23:28 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/14 16:35:45 by vlomakin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	parse_quote(char *line, int *i, t_lexems *list)
{
	(*i)++;
	while (line[*i] && line[*i] != '\'')
	{
		add_char_node(list, line[*i]);
		(*i)++;
	}
	if (line[*i] == '\'')
		(*i)++;
}

void	parse_expander_status(int *i, t_lexems *list, int *exit_status)
{
	char	*exit_str;

	exit_str = ft_itoa(*exit_status);
	add_str_node(list, exit_str);
	free(exit_str);
	(*i) += 2;
}

int	is_delimiter_or_quote(char symbol)
{
	return (is_delimiter(symbol) || symbol == '\"');
}

void	parse_double_quote(int *i, t_clean_line_args args, int *exit_status)
{
	(*i)++;
	while (args.line[*i] && args.line[*i] != '\"')
	{
		if (args.line[*i] == '$' && is_delimiter_or_quote(args.line[*i + 1]))
		{
			add_char_node(args.list, '$');
			(*i)++;
		}
		else if (args.line[*i] == '$' && args.line[*i + 1] == '?')
			parse_expander_status(i, args.list, exit_status);
		else if (args.line[*i] == '$' && args.line[*i + 1] == '0')
		{
			add_str_node(args.list, "minishell\n");
			(*i) += 2;
		}
		else if (args.line[*i] == '$')
			parse_expander_sign_in_quotes(i, args.line, args.list, args.args);
		else
		{
			add_char_node(args.list, args.line[*i]);
			(*i)++;
		}
	}
	if (args.line[*i] == '\"')
		(*i)++;
}
