/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 15:23:28 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/06/11 12:33:55 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void parse_quote(char *line, int *i, t_lexems *list)
{
    (*i)++;
    while(line[*i] && line[*i] != '\'')
    {
        add_char_node(list, line[*i]);
        (*i)++;
    }
}

void parse_double_quote(int *i, char *line, t_lexems *list, t_args *args)
{
    char *exit_str;

    (*i)++;
    while(line[*i] && line[*i] != '\"')
    {
        if (line[*i] == '$' && (is_delimiter(line[*i + 1]) || line[*i + 1] == '\"'))
        {
            add_char_node(list, '$');
            (*i)++;
        }
        else if (line[*i] == '$' && line[*i + 1] =='?')
        {
            exit_str = ft_itoa(exit_status);
            add_str_node(list, exit_str);
            free(exit_str);
            (*i) += 2;
        }
        else if (line[*i] == '$')
            parse_expander_sign(i, line, list, args);
        else
        {
            add_char_node(list, line[*i]);
            (*i)++;
        }
    }
}

