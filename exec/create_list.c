/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 17:20:39 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/14 12:29:52 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* cmd with type NONE -> expantion or heredoc without executable cmd.
It should be included into the list and into the cmd tree.
In run_exec I will skip this cmd abdo only do heredoc */

t_cmd_info	*create_cmdlist(t_cmd *cmd, t_args *args, int *exit_status)
{
	t_cmd_info	*cmd_list;

	cmd_list = NULL;
	gothrough_cmd(cmd, &cmd_list, args, exit_status);
	return (cmd_list);
}

/* creates linked list there and fill it recursively */
void	gothrough_cmd(t_cmd *cmd, t_cmd_info **cmd_list, t_args *args, int *exit_status)
{
	t_cmd_info	*cmd_node;

	cmd_node = NULL;
	if (!cmd)
		return ;
	if (cmd->type == EXEC)
		cmd_node = fill_exec(cmd);
	else if (cmd->type == REDIR)
		cmd_node = fill_redir(cmd, cmd_list, args, exit_status);
	else
		fill_pipe(cmd, cmd_list, args, exit_status);
	if (cmd_node)
		add_cmd_to_list(cmd_node, cmd_list);
	else
		*exit_status = 1;
	return ;
}

void	fill_pipe(t_cmd *cmd, t_cmd_info **cmd_list, t_args *args, int *exit_status)
{
	t_pipe	*pcmd;
	t_cmd	*left;
	t_cmd	*right;

	pcmd = (t_pipe *)cmd;
	left = pcmd->left;
	right = pcmd->right;
	free(pcmd);
	gothrough_cmd(left, cmd_list, args, exit_status);
	gothrough_cmd(right, cmd_list, args, exit_status);
}

t_cmd_info	*fill_redir(t_cmd *cmd, t_cmd_info **cmd_list, t_args *args, int *exit_status)
{
	t_redir		*rcmd;
	t_cmd_info	*new_cmd;

	rcmd = (t_redir *)cmd;
	new_cmd = malloc(sizeof(t_cmd_info));
	if (!new_cmd)
		return (free(rcmd->cmd), free(rcmd), NULL);
	define_redir_info(new_cmd, rcmd);
	if (add_redir_details(new_cmd, rcmd, args))
		return (free_redir(rcmd), free(new_cmd), (void *) NULL);
	if (new_cmd->subcmd->type == REDIR)
	{
		if (more_redir(new_cmd, rcmd, args))
			return (free_redir(rcmd), free(new_cmd), NULL);
	}
	if (new_cmd->subcmd->type == EXEC)
	{
		copy_argv(new_cmd, new_cmd->subcmd);
		copy_eargv(new_cmd, new_cmd->subcmd);
	}
	if (connection_content(new_cmd))
		return (free_redir(rcmd), free_cmd_list(new_cmd), NULL);
	if (new_cmd->subcmd->type == PIPE)
		gothrough_cmd(new_cmd->subcmd, cmd_list, args, exit_status);
	return (free(rcmd), new_cmd);
}

t_cmd_info	*fill_exec(t_cmd *cmd)
{
	t_cmd_info	*new_cmd;
	t_execcmd	*ecmd;

	new_cmd = malloc(sizeof(t_cmd_info));
	if (!new_cmd)
		return (NULL);
	ecmd = (t_execcmd *)cmd;
	new_cmd->type = EXEC;
	new_cmd->redir_type = NONE;
	copy_argv(new_cmd, cmd);
	copy_eargv(new_cmd, cmd);
	new_cmd->fd_read = 0;
	new_cmd->fd_write = 1;
	new_cmd->file_read = NULL;
	new_cmd->file_write = NULL;
	new_cmd->subcmd = 0;
	if (connection_content(new_cmd))
		return (NULL);
	free(ecmd);
	return (new_cmd);
}
