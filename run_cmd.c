/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:26:37 by vlomakin          #+#    #+#             */
/*   Updated: 2024/05/22 14:59:17 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_pipe(t_cmd *cmd)
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

void	run_redir(t_cmd *cmd)
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

void	run_exec(t_cmd *cmd)
{
	t_execcmd	*ecmd;
	char	*cmd_path;
	int	builtin_status;

	ecmd = (t_execcmd *)cmd;
	if (ecmd->argv[0] == 0)
		exit(127);
	builtin_status = run_buildin(ecmd, cmd->params);
	if (builtin_status == 0)
		return ;
	else if (builtin_status == 1)
		exit_with_err("Command not executed");
	cmd_path = find_command_path(ecmd->argv[0], cmd->params->envp);
	if(!cmd_path)
	{
		printf("Command not found: %s", ecmd->argv[0]);
		exit(127);
	}
	execve(cmd_path, ecmd->argv, cmd->params->envp);
	perror("execve");
	free(cmd_path);
	exit(1);
}

void	run_cmd(t_cmd *cmd)
{
	if (!cmd)
		exit(127);
	if (cmd->type == EXEC)
		run_exec(cmd);
	else if (cmd->type == REDIR)
		run_redir(cmd);
	else if (cmd->type == PIPE)
		run_pipe(cmd);
}

