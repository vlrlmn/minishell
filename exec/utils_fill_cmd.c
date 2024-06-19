/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_fill_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 14:27:19 by sabdulki          #+#    #+#             */
/*   Updated: 2024/06/19 16:30:26 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	copy_eargv(t_cmd_info *new_cmd, t_cmd *cmd)
{
	t_execcmd	*ecmd;
	t_redir		*rcmd;
	t_pipe		*pcmd;
	int	i;

	i = -1;
	if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		while (i < MAXARGS && ecmd->eargv[++i])
        	new_cmd->eargv[i] = ecmd->eargv[i];
    }
    else if (cmd->type == REDIR)
	{
		rcmd = (t_redir *)cmd;
		while (i < MAXARGS && rcmd->eargv[++i])
        	new_cmd->eargv[i] = rcmd->eargv[i];
	}
    else if (cmd->type == PIPE)
	{
		pcmd = (t_pipe *)cmd;
		while (i < MAXARGS && pcmd->eargv[++i])
        	new_cmd->eargv[i] = pcmd->eargv[i];
	}
}

void	copy_argv(t_cmd_info *new_cmd, t_cmd *cmd)
{
	t_execcmd	*ecmd;
	t_redir		*rcmd;
	t_pipe		*pcmd;
	int	i;

	i = -1;
	if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		while (i < MAXARGS && ecmd->argv[++i])
        	new_cmd->argv[i] = ecmd->argv[i];
    }
    else if (cmd->type == REDIR)
	{
		rcmd = (t_redir *)cmd;
		while (i < MAXARGS && rcmd->argv[++i])
        	new_cmd->argv[i] = rcmd->argv[i];
	}
    else if (cmd->type == PIPE)
	{
		pcmd = (t_pipe *)cmd;
		while (i < MAXARGS && pcmd->argv[++i])
        	new_cmd->argv[i] = pcmd->argv[i];
	}
}