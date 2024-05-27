/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aajaanan <aajaanan@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 09:09:04 by aajaanan          #+#    #+#             */
/*   Updated: 2023/09/01 20:10:24 by aajaanan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/command.h"

t_cmd	*pipecmd(t_cmd *left, t_cmd *right)
{
	t_pipecmd	*cmd;

	cmd = (t_pipecmd *)malloc(sizeof(t_pipecmd));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((t_cmd *)cmd);
}

t_cmd	*execcmd(void)
{
	t_execcmd	*cmd;

	cmd = (t_execcmd *)malloc(sizeof(t_execcmd));
	ft_memset(cmd, 0, sizeof(t_execcmd));
	cmd->type = EXEC;
	return ((t_cmd *)cmd);
}

t_redirdata	redirdata(char *file, char *efile, int mode, int fd)
{
	t_redirdata	redirdata;

	redirdata.file = file;
	redirdata.efile = efile;
	redirdata.mode = mode;
	redirdata.fd = fd;
	return (redirdata);
}

t_cmd	*redircmd(t_cmd *subcmd, t_redirdata data, char r_type)
{
	t_redircmd	*cmd;

	cmd = (t_redircmd *)malloc(sizeof(t_redircmd));
	cmd->type = REDIR;
	cmd->file = data.file;
	cmd->efile = data.efile;
	cmd->mode = data.mode;
	cmd->fd = data.fd;
	cmd->r_type = r_type;
	cmd->subcmd = subcmd;
	return ((t_cmd *)cmd);
}
