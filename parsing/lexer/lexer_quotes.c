/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 15:23:28 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/13 21:13:30 by sabdulki         ###   ########.fr       */
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

void	parse_expander_status(int *i, t_lexems *list)
{
	char	*exit_str;

	exit_str = ft_itoa(g_exit_status);
	add_str_node(list, exit_str);
	free(exit_str);
	(*i) += 2;
}

void	parse_double_quote(int *i, char *line, t_lexems *list, t_args *args)
{
	(*i)++;
	while (line[*i] && line[*i] != '\"')
	{
		if (line[*i] == '$' && (is_delimiter(line[*i + 1]) || \
		line[*i + 1] == '\"'))
		{
			add_char_node(list, '$');
			(*i)++;
		}
		else if (line[*i] == '$' && line[*i + 1] == '?')
			parse_expander_status(i, list);
		else if (line[*i] == '$' && line[*i + 1] == '0')
		{
			add_str_node(list, "minishell\n");
			(*i) += 2;
		}
		else if (line[*i] == '$')
			parse_expander_sign_in_quotes(i, line, list, args);
		else
		{
			add_char_node(list, line[*i]);
			(*i)++;
		}
	}
	if (line[*i] == '\"')
		(*i)++;
}
