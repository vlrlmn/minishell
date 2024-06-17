/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 12:38:32 by vlomakin          #+#    #+#             */
/*   Updated: 2024/06/17 19:28:55 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*redircmd(t_cmd *subcmd, char *file, char *efile, int mode, int fd, int subtype)
{
	t_redir	*redircmd;
	char 	original_efile_char;

	redircmd = malloc(sizeof(*redircmd));
	if (!redircmd)
		exit_with_malloc_error(MALLOC_ERROR);
	ft_memset(redircmd, 0, sizeof(redircmd));
	redircmd->type = REDIR;
	redircmd->subtype = subtype;
	redircmd->cmd = subcmd; //points to the sub-command that is being redirected
	redircmd->file = file; // if HEREDOC or APPEND subtype, it's limiter, not file.
	redircmd->efile = efile;
	redircmd->mode = mode;
	redircmd->fd = fd;
	fprintf(stderr, "fd in parse_redir: %d\n", redircmd->fd);

	// Save the original character pointed to by efile
    original_efile_char = *redircmd->efile;
	// Set efile to 0 before calling heredoc
	*redircmd->efile = 0;
	if (subtype == 3)
	{
		// do the heredoc -> get the input from user and write it to tmp file, redir fds.
		if (heredoc(redircmd))
			printf("heredoc failed!\n"); //and return NULL
	}
	*redircmd->efile = original_efile_char;
	fprintf(stderr, "fd after heredoc: %d\n", redircmd->fd);
	return ((t_cmd *)redircmd);
}

t_cmd	*parseredir(t_cmd *cmd, char **ps, char *es)
{
	int		tok;
	char	*q;
	char	*eq;
	
	while (peek(ps, es, "<>"))
	{
		tok = gettoken(ps, es, &q, &eq);
		if (tok == '<')
			cmd = redircmd(cmd, q, eq, O_RDONLY, 0, REDIR);
		else if (tok == '>')
			cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_TRUNC, 1, REDIR);
		else if (tok == '+') // it's << actually
			cmd = redircmd(cmd, q, eq, O_RDWR, 0, HEREDOC);
		else if (tok == '-') // >>
			cmd = redircmd(cmd, q, eq, O_RDONLY, 1, APPEND);
		printf("\n--------PS: %s --------\n", *ps);
        printf("\n--------ES: %s --------\n", es);
        printf("\n--------Q: %.*s --------\n", (int)(eq - q), q); // Properly print the token
        printf("\n--------EQ: %s --------\n", eq);
	}
	return (cmd);
}


