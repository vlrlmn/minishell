/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:26:37 by vlomakin          #+#    #+#             */
/*   Updated: 2024/05/16 18:11:21 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_pipe(t_cmd **cmd)
{
	t_pipe	*pcmd;
	int	p[2];

	pcmd = (t_pipe *)cmd;
	if (pipe(p) < 0)
		exit_with_err("Pipe error");
	if (fork1() == 0)
	{
		close(1);
		dup(p[1]);
		close(p[0]);
		close(p[1]);
		run_cmd(pcmd->left);
	}
	if (fork1() == 0)
	{
		close(0);
		dup(p[0]);
		close(p[0]);
		close(p[1]);
		run_cmd(pcmd->right);
	}
	close(p[0]);
	close(p[1]);
	wait(0);
	wait(0);
}

void	run_redir(t_cmd **cmd)
{
	t_redir *rcmd;

	rcmd = (t_redir *)cmd;
	close(rcmd->fd);
	if(open(rcmd->file, rcmd->mode) < 0)
	{
		printf("open %s failed\n", rcmd->file);
		exit(126);
	}
	run_cmd(rcmd->cmd);
}

void	run_exec(t_cmd **cmd)
{
	t_execcmd	*ecmd;

	ecmd = (t_execcmd *)*cmd;
	if (ecmd->argv[0] == 0)
		exit(127);
	execve(ecmd->argv[0], ecmd->argv, ecmd->params->envp);
	printf("exec %s failed\n", ecmd->argv[0]);
}

void	run_cmd(t_cmd *cmd)
{
	if (!cmd)
		exit(127);
	if (cmd->type == EXEC)
		run_exec(&cmd);
	else if (cmd->type == REDIR)
		run_redir(&cmd);
	else if (cmd->type == PIPE)
		run_pipe(&cmd);
}

