/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:52:25 by vlomakin          #+#    #+#             */
/*   Updated: 2024/06/13 16:15:38 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* add condition for << and >> */
// 'q' marks the beginning of the token.
// 'eq' marks the end of token

int	is_delimiter(char c)
{
	return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ');
}

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
            while (*ps < es && is_delimiter(**ps))
                (*ps)++;
            *q = *ps; // mark the start of limiter name
            while (*ps < es && !is_delimiter(**ps) && !ft_strchr("|<>", **ps))
                (*ps)++;
            *eq = *ps;
            token = '+';
            return (token);
        }
        else if (**ps == '>')
        {
            (*ps)++;
            while (*ps < es && is_delimiter(**ps))
                (*ps)++;
            *q = *ps; // mark the start of limiter name
            while (*ps < es && !is_delimiter(**ps) && !ft_strchr("|<>", **ps))
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

t_redir	*redircmd(t_redir *redircmd, char *file, char *efile, int mode, int fd, int subtype)
{
	redircmd->type = REDIR;
	redircmd->subtype = subtype;
	// redircmd->cmd = subcmd; //points to the sub-command that is being redirected
	redircmd->file = file; // if HEREDOC or APPEND subtype, it's limiter, not file. 
	redircmd->efile = efile;
	redircmd->mode = mode;
	redircmd->fd = fd;
	// if (subtype == 3)
	// {
	// 	// do the heredoc -> get the input from user and write it to tmp file, redir fds.
	// 	if (heredoc(redircmd))
	// 		printf("heredoc failed!\n"); //and return NULL
	// }
	fprintf(stderr, "\nFILE: %s\n", redircmd->file);
	fprintf(stderr, "\neFILE: %s\n", redircmd->efile);
    
	return (redircmd);
}

int main(int ac, char **av)
{
    int		tok;
	char	*q;
	char	*eq;
    t_redir *cmd;
    (void)ac;
    
    char    *ps = av[1];
    char	*es;
    es = ps + ft_strlen(av[1]);

    cmd = malloc(sizeof(t_redir));
	if (!cmd)
		return (1);
	ft_memset(cmd, 0, sizeof(cmd));

    while (peek(&ps, es, "<>"))
	{
		tok = gettoken(&ps, es, &q, &eq);
		if (tok == '<')
			cmd = redircmd(cmd, q, eq, O_RDONLY, 0, REDIR);
		else if (tok == '>')
			cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_TRUNC, 1, REDIR);
		else if (tok == '+') // it's << actually
			cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT, 1, HEREDOC);
		else if (tok == '-') // >>
			cmd = redircmd(cmd, q, eq, O_RDONLY, 1, APPEND);
		// printf("\n--------PS: %s --------\n", *ps);
        // printf("\n--------ES: %s --------\n", es);
        // printf("\n--------Q: %.*s --------\n", (int)(eq - q), q); // Properly print the token
        // printf("\n--------EQ: %s --------\n", eq);
	}
    return (0);
}

