/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:52:25 by vlomakin          #+#    #+#             */
/*   Updated: 2024/07/02 22:06:37 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* add condition for << and >> */
// 'q' marks the beginning of the token.
// 'eq' marks the end of token
int gettoken(char **ps, char *es, char **q, char **eq) // we got 3 addresses of pointers
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
            while (is_delimiter(**ps))
                (*ps)++;
            *q = *ps; // mark the start of limiter name
            while (!is_delimiter(**ps) && !ft_strchr("|<>", **ps))
                (*ps)++;
            *eq = *ps;
            token = '+';
            return (token);
        }
        else if (**ps == '>')
        {
            (*ps)++;
            while (is_delimiter(**ps))
                (*ps)++;
            *q = *ps; // mark the start of limiter name
            while (!is_delimiter(**ps) && !ft_strchr("|<>", **ps))
                (*ps)++;
            *eq = *ps;
            token = '-';
            return (token);
        }
        while (is_delimiter(**ps))
            (*ps)++;
        *q = *ps;
        while (!is_delimiter(**ps) && !ft_strchr("|<>", **ps))
            (*ps)++;
        *eq = *ps;
        return token;
    }
    //////////
    *q = *ps;
    while (*ps < es && !is_delimiter(**ps) && !ft_strchr("<>|", **ps))
    {
        if (**ps == '"')
        {
            (*ps)++;
            while(*ps < es && **ps != '"')
                (*ps)++;
            if (*ps >= es)
                return (0);
            (*ps)++;
        }
        else if (**ps == '\'')
        {
            (*ps)++;
            while(*ps < es && **ps != '\'')
                (*ps)++;
            if (*ps >= es)
                return (0);
            (*ps)++;    
        }
        else if (*ps < es)
            (*ps)++;
    }
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

