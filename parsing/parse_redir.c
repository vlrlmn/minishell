/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 12:38:32 by vlomakin          #+#    #+#             */
/*   Updated: 2024/06/04 21:21:47 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*redircmd(t_cmd *subcmd, char *file, char *efile, int mode, int fd)
{
	t_redir	*redircmd;

	redircmd = malloc(sizeof(*redircmd));
	if (!redircmd)
		exit_with_malloc_error(MALLOC_ERROR);
	ft_memset(redircmd, 0, sizeof(redircmd));
	redircmd->type = REDIR;
	redircmd->cmd = subcmd;
	redircmd->file = file;
	redircmd->efile = efile;
	redircmd->mode = mode;
	redircmd->fd = fd;
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
			cmd = redircmd(cmd, q, eq, O_RDONLY, 0);
		else if (tok == '>')
			cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_TRUNC, 1);
		else if (tok == '+')
			cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT, 1);
		else if (tok == '-')
			cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT, 1);
		printf("\n--------PS: %s --------\n", *ps);
        printf("\n--------ES: %s --------\n", es);
        printf("\n--------Q: %.*s --------\n", (int)(eq - q), q); // Properly print the token
        printf("\n--------EQ: %s --------\n", eq);
	}
	return (cmd);
}
