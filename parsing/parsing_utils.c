/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:52:25 by vlomakin          #+#    #+#             */
/*   Updated: 2024/06/12 17:22:10 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* add condition for << and >> */
// 'q' marks the beginning of the token.
// 'eq' marks the end of token
int gettoken(char **ps, char *es, char **q, char **eq)
{
    int token;
    while (*ps < es && (**ps == ' ' || **ps == '\t' || **ps == '\n'))
        (*ps)++;
    if (*ps >= es)
        return 0;
    token = **ps;
    if (**ps == '|')
    {
        (*ps)++;
        *eq = *ps;
        return token;
    }
    else if (**ps == '<' || **ps == '>')
    {
        (*ps)++;
        if (**ps == '<')
        {
            (*ps)++;
            token = '+';
            return (token);
        }
        else if (**ps == '>')
        {
            (*ps)++;
            token = '-';
            return (token);
        }
        while(is_delimiter(**ps))
            (*ps)++;
        *q = *ps;
        while(!is_delimiter(**ps) && !ft_strchr("|<>", **ps))
            (*ps)++;
        *eq = *ps;
        return token;
    }
    if (**ps == '"')
    {
        (*ps)++;
        *q = *ps;
        while(*ps < es && **ps != '"')
            (*ps)++;
        if (*ps >= es)
            return (0);
        *eq = *ps;
        (*ps)++;
        return ('a');
    }
    else if (**ps == '\'')
    {
        (*ps)++;
        *q = *ps;
        while(*ps < es && **ps != '\'')
            (*ps)++;
        if (*ps >= es)
            return (0);
        *eq = *ps;
        (*ps)++;
        return ('a');
    }
    *q = *ps;
    while (*ps < es && (**ps != ' ' && **ps != '\t' && **ps != '\n') && !ft_strchr("<>|", **ps))
        (*ps)++;
    *eq = *ps;
    return ('a');
}

int	peek(char **ps, char *es, char *toks)
{
	char	*s;

	s = *ps;
	while (s < es && is_delimiter(*s))
		s++;
	*ps = s;
    /* After the loop, *ps is updated to point to the current position of s. 
    This modifies the original ps pointer in the calling function to reflect the new position, past any leading delimiters.
    */
	return (*s && ft_strchr(toks, *s)); //Checks if the current character is a valid token
    /* The function returns the result of the logical AND between these two conditions. 
    If both are true (i.e., *s is not '\0' and *s is in toks), the function returns a non-zero value (true). 
    Otherwise, it returns zero (false). */
}

