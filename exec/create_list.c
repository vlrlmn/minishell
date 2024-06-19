/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 17:20:39 by sabdulki          #+#    #+#             */
/*   Updated: 2024/06/19 16:52:22 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd_info	*create_cmdlist(t_cmd *cmd)
{
	t_cmd_info *cmd_list;

	cmd_list = NULL;
	gothrough_cmd(cmd, &cmd_list);
	return (cmd_list);
}

void	gothrough_cmd(t_cmd *cmd, t_cmd_info **cmd_list)
{
	t_cmd_info *cmd_node;

	cmd_node = NULL;
    if (!cmd)
	{
        printf("run_cmd: cmd is NULL\n");
		return ;
	}
    fprintf(stderr, "Running command type: %d\n", cmd->type);
	// create linked list there and fill it recursively
	// ATTENTION:  it will not work until it gets only one structure
    if (cmd->type == EXEC) //builtin or any usual cmd
        cmd_node = fill_exec(cmd);
    else if (cmd->type == REDIR)
        cmd_node = fill_redir(cmd, cmd_list);
    else if (cmd->type == PIPE)
        fill_pipe(cmd, cmd_list);
	if (cmd_node)
		add_cmd_to_list(cmd_node, cmd_list);
	return ;
}

/*
Memory Allocation: Ensure malloc is properly used and sufficient memory is allocated.
Pointer Dereferencing: Ensure pointers are valid before dereferencing.
Buffer Copy Functions: Ensure functions like copy_argv and copy_eargv do not exceed buffer limits.
*/
void	fill_pipe(t_cmd *cmd, t_cmd_info **cmd_list)
{
	t_pipe		*pcmd;
	// t_cmd_info	*new_cmd;

	pcmd = (t_pipe *)cmd;
	// new_cmd = malloc(sizeof(t_cmd_info));
	// if (!new_cmd)
	// 	return (NULL);
	// // FT_MEMSET FOR new_cmd!
	// new_cmd->type = PIPE;
	// copy_argv(new_cmd, cmd);
	// copy_eargv(new_cmd, cmd);
	// if (pcmd->left->type == PIPE)
	// {
		// не записывать в лист, а отправить в рекурсию
		gothrough_cmd(pcmd->left, cmd_list);
	// }
	// else if (pcmd->right->type == PIPE)
		gothrough_cmd(pcmd->right, cmd_list);
	// return (new_cmd);
}

t_cmd_info	*fill_redir(t_cmd *cmd, t_cmd_info **cmd_list)
{
	t_redir		*rcmd;
	t_cmd_info	*new_cmd;

	rcmd = (t_redir *)cmd; 
	new_cmd = malloc(sizeof(t_cmd_info));
	if (!new_cmd)
		return (NULL);
	// FT_MEMSET FOR new_cmd!
	new_cmd->type = rcmd->type;
	copy_argv(new_cmd, cmd);
	copy_eargv(new_cmd, cmd);
	// add this subcmd there
	new_cmd->subcmd = rcmd->cmd;
	new_cmd->redir_type = rcmd->subtype;
	// if ()
	// new_cmd->fd_read = rcmd->fd_read;
	// new_cmd->fd_write = rcmd->fd_write;
	// new_cmd->file_read = rcmd->file_read;
	// new_cmd->file_write = rcmd->file_write;
	//add smth else??
	if (new_cmd->subcmd->type == PIPE)
		gothrough_cmd (new_cmd->subcmd, cmd_list);
	if (new_cmd->subcmd->type == REDIR)
		return (fill_redir(new_cmd->subcmd, cmd_list));
	// if (new_cmd->subcmd == EXEC)
	// return (fill_exec(new_cmd->subcmd)); //one more node SOLVE EXTRA NODE PROBLEM
	return (new_cmd);
}

t_cmd_info	*fill_exec(t_cmd *cmd)
{
	t_execcmd	*ecmd;
	t_cmd_info	*new_cmd;

	ecmd = (t_execcmd *)cmd;
	new_cmd = malloc(sizeof(t_cmd_info));
	if (!new_cmd)
		return (NULL);
	check_arguments(ecmd);
	// FT_MEMSET FOR new_cmd!
	new_cmd->type = EXEC;
	copy_argv(new_cmd, cmd);
	copy_eargv(new_cmd, cmd);
	new_cmd->fd_read = 0; //change if redirs
	new_cmd->fd_write = 1; //change if redirs
	// new_cmd->connection //create a pipe for connectiona and fill it smwh else
	return (new_cmd);
}
