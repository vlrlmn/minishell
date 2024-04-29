/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 11:50:56 by vlomakin          #+#    #+#             */
/*   Updated: 2024/04/29 19:23:44 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/*Debugging*/
const char *token_type_string(t_type type) {
    switch (type) {
        case WORD: return "WORD";
        case PIPE: return "PIPE";
        case REDIR_IN: return "REDIR_IN";
        case REDIR_OUT: return "REDIR_OUT";
        default: return "UNKNOWN";
    }
}

void print_tokens(const t_token *tokens) {
    const t_token *current = tokens;
    printf("Tokens:\n");
    while (current != NULL) {
        printf("Type: %s, Value: %s\n", token_type_string(current->type), current->value);
        current = current->next;
    }
}
/* ^^^^^^^^Debugging^^^^^^^^*/

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
	print_tokens(head); //Debugging
}
