/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 12:38:32 by vlomakin          #+#    #+#             */
/*   Updated: 2024/07/04 18:18:53 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*redircmd(t_cmd *subcmd, char *file, char *efile, int mode, int fd, int subtype)
{
	t_redir	*redircmd;

	redircmd = malloc(sizeof(*redircmd));
	if (!redircmd)
		exit_with_malloc_error(MALLOC_ERROR);
	ft_memset(redircmd, 0, sizeof(redircmd));
	redircmd->type = REDIR;
	redircmd->subtype = subtype;
	redircmd->cmd = subcmd; //points to the sub-command that is being redirected
	redircmd->file = file;
	redircmd->efile = efile;
	redircmd->mode = mode;
	redircmd->fd = fd; //the fd that was passed as argument is affecting to the heredoc!!!!
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
			cmd = redircmd(cmd, q, eq, O_RDONLY, 0, REDIRIN);
		else if (tok == '>')
			cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_TRUNC, 1, REDIROUT);
		else if (tok == '+') // it's << actually
			cmd = redircmd(cmd, q, eq, O_RDWR | O_CREAT, 0, HEREDOC);
		else if (tok == '-') // >>
			cmd = redircmd(cmd, q, eq,  O_WRONLY | O_CREAT | O_APPEND, 1, APPEND);
		// printf("\n--------PS: %s --------\n", *ps);
        // printf("\n--------ES: %s --------\n", es);
        // printf("\n--------Q: %.*s --------\n", (int)(eq - q), q); // Properly print the token
        // printf("\n--------EQ: %s --------\n", eq);
	}
	return (cmd);
}

