/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 11:50:56 by vlomakin          #+#    #+#             */
/*   Updated: 2024/04/29 18:27:04 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	in_quotes_token(char *line, int pos, char sym, t_token *tok)
{
	int	end_pos;
	int	start_pos;
	char	end_char;

	end_char = ft_strrchr(line, sym);
	end_pos = 0;
	start_pos = 0;
	while (line[end_pos] && line[end_pos] != end_char)
		end_pos++;
	tok->value = malloc(sizeof(char) * end_pos + 1);
	if(!tok->value)
		exit (EX_UNAVAILABLE);
	tok->len = ft_strlcpy(tok->value, line, end_pos);
	tok->pos = pos;
	tok->type = WORD_IN_QUOTES;
	return(end_pos);
}

void	symbol_token(char *symbol, int pos, t_token *tok)
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
	tok->len = ft_strlcpy(tok->value, symbol, 1);
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
	return(end);
}

int fill_new_token(char *input, t_token *last, int i)
{
	if (input[i] == 34 || input[i] == 39)
		i += in_quotes_token(input + i, i, input[i], last);
	else if (input[i] == '>' || input[i] == '<' || input[i] == '|')
	{
		symbol_token(input[i], i, last);
		return (i + 1);
	}
	else if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' 
			&& input[i + 1] == '<'))
	{
		two_symbols_token(input + i, 2, i, last);
		return (i + 2);
	}
	else if (input[i] == '$')
		return (i += expansion_token(input + i, i, last));
	else if	(!is_delimiter(input[i]))
		return (i += word_token(input + i, i, last));
	while (input[i] && is_delimiter(input[i]))
	   	i++;
	return (i);
}

void	lexer(char *input)
{
	int	i;
	t_token *head;
	t_token *last;
	t_token *new;

	head = NULL;
	last = NULL;
	i = 0;
	while (input[i])
	{
		new = malloc(sizeof(t_token));
		if (!new)
		{
			free(head);
			return ;
		}
		new->next = NULL;
		if (!head)
			head = new;
		else
			last->next = new;
		last = new;
		i = fill_new_token(input, new, i);
	}
}
