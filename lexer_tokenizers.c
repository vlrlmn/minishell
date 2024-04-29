/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokenizers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 19:14:25 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/04/29 19:26:32 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	in_quotes_token(char *line, int pos, char sym, t_token *tok)
{
	int	end_pos;
	char*	end_char;

	end_char = ft_strrchr(line, sym);
	end_pos = 0;
	while (line[end_pos] && line[end_pos] != end_char[0])
		end_pos++;
	tok->value = malloc(sizeof(char) * end_pos + 1);
	if(!tok->value)
		exit (EX_UNAVAILABLE);
	tok->len = ft_strlcpy(tok->value, line, end_pos);
	tok->pos = pos;
	tok->type = WORD_IN_QUOTES;
	return(end_pos);
}

void	symbol_token(char symbol, int pos, t_token *tok)
{
	if (symbol == '<')
		tok->type = REDIR_IN;
	else if(symbol == '>')
		tok->type = REDIR_OUT;
	else if(symbol == '|')
		tok->type = PIPE;
	tok->pos = pos;
	tok->value = malloc(sizeof(char) * 2);
	if(!tok->value)
		exit (EX_UNAVAILABLE);
	tok->len = ft_strlcpy(tok->value, &symbol, 1);
}

void	two_symbols_token(char *symbols, int len, int pos, t_token *tok)
{
	if (symbols[0] == '<')
		tok->type = HEREDOC;
	else
		tok->type = REDIR_APP;
	tok->pos = pos;
	tok->value = malloc(sizeof(char) * len + 1);
	if(!tok->value)
		exit (EX_UNAVAILABLE);
	tok->len = ft_strlcpy(tok->value, symbols, len);
}

int	expansion_token(char *work_line, int pos, t_token *tok)
{
	int	end;

	end = 0;
	while (work_line[end] && !is_delimiter(work_line[end]))
		end++;
	tok->value = malloc(sizeof(char) * (end + 1));
	if(!tok->value)
		exit (EX_UNAVAILABLE);
	tok->len = ft_strlcpy(tok->value, work_line, end);
	tok->pos = pos;
	tok->type = EXPANSION;
	return (end);
}

int	word_token(char *work_line, int pos, t_token *tok)
{
	int end;

	end = 0;
	while (work_line[end] && !is_delimiter(work_line[end]))
		end++;
	tok->value = malloc(sizeof(char) * (end + 1));
	if(!tok->value)
		exit (EX_UNAVAILABLE);
	tok->len = ft_strlcpy(tok->value, work_line, end);
	tok->type = WORD;
	tok->pos = pos;
	return(end);
}
