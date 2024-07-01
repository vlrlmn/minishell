/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 17:20:39 by sabdulki          #+#    #+#             */
/*   Updated: 2024/06/28 19:02:38 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd_info	*create_cmdlist(t_cmd *cmd,  t_args *args)
{
	t_cmd_info *cmd_list;

	cmd_list = NULL;
	gothrough_cmd(cmd, &cmd_list, args);
	return (cmd_list);
}

void	gothrough_cmd(t_cmd *cmd, t_cmd_info **cmd_list, t_args *args)
{
	t_cmd_info *cmd_node;

	cmd_node = NULL;
    if (!cmd)
	{
        // printf("run_cmd: cmd is NULL\n");
		return ;
	}
    // fprintf(stderr, "Running command type: %d\n", cmd->type);
	// create linked list there and fill it recursively
    if (cmd->type == EXEC) //builtin or any usual cmd
        cmd_node = fill_exec(cmd);
    else if (cmd->type == REDIR)
        cmd_node = fill_redir(cmd, cmd_list, args);
    else //PIPE type
        fill_pipe(cmd, cmd_list, args);
    // else if (cmd->type == PIPE) //PIPE type
	if (cmd_node)
		add_cmd_to_list(cmd_node, cmd_list);
	return ;
}

/*
Memory Allocation: Ensure malloc is properly used and sufficient memory is allocated.
Pointer Dereferencing: Ensure pointers are valid before dereferencing.
Buffer Copy Functions: Ensure functions like copy_argv and copy_eargv do not exceed buffer limits.
*/
void	fill_pipe(t_cmd *cmd, t_cmd_info **cmd_list, t_args *args)
{
	t_pipe		*pcmd;

	pcmd = (t_pipe *)cmd;
		// не записывать в лист, а отправить в рекурсию
	gothrough_cmd(pcmd->left, cmd_list, args);
	gothrough_cmd(pcmd->right, cmd_list, args);
}

t_cmd_info	*fill_redir(t_cmd *cmd, t_cmd_info **cmd_list, t_args *args)
{
	t_redir		*rcmd;
	t_cmd_info	*new_cmd;

	rcmd = (t_redir *)cmd; 
	new_cmd = malloc(sizeof(t_cmd_info));
	if (!new_cmd)
		return (NULL);
	// FT_MEMSET FOR new_cmd! ??
	new_cmd->type = rcmd->type;
	new_cmd->subcmd = rcmd->cmd;
	copy_argv(new_cmd, new_cmd->subcmd);
	copy_eargv(new_cmd, new_cmd->subcmd);
	new_cmd->file_read = NULL;
	new_cmd->file_write = NULL;
	new_cmd->hfile_array = NULL;
	new_cmd->fd_read = 0;
	new_cmd->fd_write = 1;
	// rcmd->mode_read = 0 ???
	// rcmd->mode_write = 0 ???
	if (connection_content(new_cmd))
		return (NULL);
	if (add_redir_details(new_cmd, rcmd, args))
		return (NULL);
	// printf("new_cmd->subcmd->type: %d\n", new_cmd->subcmd->type);
	if (new_cmd->subcmd->type == PIPE)
		gothrough_cmd(new_cmd->subcmd, cmd_list, args);
	//it should be smth like recursive, because there could be more than 2 redirs!
	if (new_cmd->subcmd->type == REDIR) 
	{
		more_redir(new_cmd, rcmd, args); // free
	}
	free(rcmd);
	return (new_cmd);
}

t_cmd_info	*fill_exec(t_cmd *cmd)
{
	t_cmd_info	*new_cmd;
	t_execcmd	*ecmd;

	new_cmd = malloc(sizeof(t_cmd_info));
	if (!new_cmd)
		return (NULL);
	ecmd = (t_execcmd *)cmd;
	// check_arguments(new_cmd);
	// FT_MEMSET FOR new_cmd!
	new_cmd->type = EXEC;
	new_cmd->redir_type = NONE;
	copy_argv(new_cmd, cmd);
	copy_eargv(new_cmd, cmd);
	new_cmd->fd_read = 0; //change if redirs
	new_cmd->fd_write = 1; //change if redirs
	new_cmd->file_read = NULL;
	new_cmd->file_write = NULL;
	new_cmd->hfile_array = NULL;
	new_cmd->subcmd = 0;
	if (connection_content(new_cmd))
		return (NULL);
	free(ecmd);
	return (new_cmd);
}
