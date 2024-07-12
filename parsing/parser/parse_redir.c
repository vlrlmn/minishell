/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 12:38:32 by vlomakin          #+#    #+#             */
/*   Updated: 2024/07/12 16:23:45 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*redir_append(t_cmd *subcmd, char *file, char *efile)
{
	t_redir	*redircmd;

	redircmd = malloc(sizeof(*redircmd));
	if (!redircmd)
		exit_with_malloc_error(MALLOC_ERROR);
	ft_memset(redircmd, 0, sizeof(redircmd));
	redircmd->type = REDIR;
	redircmd->subtype = APPEND;
	redircmd->cmd = subcmd;
	redircmd->file = file;
	redircmd->efile = efile;
	redircmd->mode = O_WRONLY | O_CREAT | O_APPEND;
	redircmd->fd = 1;
	return ((t_cmd *)redircmd);
}

t_cmd	*redir_heredoc(t_cmd *subcmd, char *file, char *efile)
{
	t_redir	*redircmd;

	redircmd = malloc(sizeof(*redircmd));
	if (!redircmd)
		exit_with_malloc_error(MALLOC_ERROR);
	ft_memset(redircmd, 0, sizeof(redircmd));
	redircmd->type = REDIR;
	redircmd->subtype = HEREDOC;
	redircmd->cmd = subcmd;
	redircmd->file = file;
	redircmd->efile = efile;
	redircmd->mode = O_RDWR | O_CREAT;
	redircmd->fd = 0;
	return ((t_cmd *)redircmd);
}

t_cmd	*redirout(t_cmd *subcmd, char *file, char *efile)
{
	t_redir	*redircmd;

	redircmd = malloc(sizeof(*redircmd));
	if (!redircmd)
		exit_with_malloc_error(MALLOC_ERROR);
	ft_memset(redircmd, 0, sizeof(redircmd));
	redircmd->type = REDIR;
	redircmd->subtype = REDIROUT;
	redircmd->cmd = subcmd;
	//redircmd->file = ft_strdup(file); ???
	redircmd->file = file;
	redircmd->efile = efile;
	redircmd->mode = O_WRONLY | O_CREAT | O_TRUNC;
	redircmd->fd = 1;
	return ((t_cmd *)redircmd);
}

t_cmd	*redirin(t_cmd *subcmd, char *file, char *efile)
{
	t_redir	*redircmd;

	redircmd = malloc(sizeof(*redircmd));
	if (!redircmd)
		exit_with_malloc_error(MALLOC_ERROR);
	ft_memset(redircmd, 0, sizeof(redircmd));
	redircmd->type = REDIR;
	redircmd->subtype = REDIRIN;
	redircmd->cmd = subcmd;
	redircmd->file = file;
	redircmd->efile = efile;
	redircmd->mode = O_RDONLY;
	redircmd->fd = 0;
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
			cmd = redirin(cmd, q, eq);
		else if (tok == '>')
			cmd = redirout(cmd, q, eq);
		else if (tok == '+')
			cmd = redir_heredoc(cmd, q, eq);
		else if (tok == '-')
			cmd = redir_append(cmd, q, eq);
	}
	return (cmd);
}
