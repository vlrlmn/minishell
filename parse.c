/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlomakin <vlomakin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 11:50:56 by vlomakin          #+#    #+#             */
/*   Updated: 2024/04/26 17:20:59 by vlomakin         ###   ########.fr       */
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
	tok->len = ft_strlcpy(tok->value, line, end_pos);
	tok->pos = pos;
	tok->type = WORD_IN_QUOTES;
	tok->next = NULL;
	return(end_pos);
}

void	symbol_token(char *symbol, int pos, t_token *tok)
{
	tok->value = symbol;
	if (symbol == '<')
		tok->type = REDIR_IN;
	else if(symbol == '>')
		tok->type = REDIR_OUT;
	else if(symbol == '|')
		tok->type = PIPE;
	tok->len = 1;
	tok->pos = pos;
	tok->next = NULL;
}

void	two_symbols_token(char *symbols, int len, t_token *tok, int pos)
{
	tok->len = ft_strlcpy(tok->value, symbols, len);
	if (symbols[0] == '<')
		tok->type = HEREDOC;
	else
		tok->type = REDIR_APP;
	tok->pos = pos;
	tok->next = NULL;
}

int	expansion_token(char *work_line, int pos, t_token *tok)
{
	int	end;

	end = 0;
	while (work_line[end] && !is_delimiter(work_line[end]))
		end++;
	tok->len = ft_strlcpy(tok->value, work_line, end);
	tok->pos = pos;
	tok->type = EXPANSION;
	tok->next = NULL;
	return (end);
}

int	word_token(char *work_line, int pos, t_token *tok)
{
	int end;

	end = 0;
	while (work_line[end] && !is_delimiter(work_line[end]))
		end++;
	tok->len = ft_strlcpy(tok->value, work_line, end);
	tok->type = WORD;
	tok->next = NULL;
	return(end);
}

void	lexer(char *input)
{
	t_token	*tokens;
	int	i;

	i = 0;
	tokens = malloc(sizeof(t_token));
	if (!tokens)
		return (NULL);
	while (input[i])
	{
		if (input[i] == 34 || input[i] == 39)
			i += in_quotes_token(input + i, i, input[i], tokens);
		else if (input[i] == '>' || input[i] == '<' || input[i] == '|')
		{
			symbol_token(input[i], i, tokens);
			i++;
		}
		else if ((input[i] == '>' && input[i + 1] == '>' && input[i + 1])
			|| (input[i] == '<' && input[i + 1] == '<' && input[i + 1]))
		{
			two_symbols_token(input + i, 2, tokens, i);
			i += 2;
		}
		else if (input[i] == '$')
			i += expansion_token(input + i, i, tokens);
		else if	(!is_delimiter(input[i]))
			i += word_token(input + i, i, tokens);
	} // FIX i counter
}
