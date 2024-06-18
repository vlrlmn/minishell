/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 17:20:39 by sabdulki          #+#    #+#             */
/*   Updated: 2024/06/18 19:46:46 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd_info *create_cmdlist(t_cmd *cmd, t_cmd *params)
{
	t_cmd_info *cmd_list;
	
	cmd_list = malloc(sizeof(t_cmd_info *));
	cmd_list->head = 0;
	gothrough_cmd(cmd, params, cmd_list);
	
}

void gothrough_cmd(t_cmd *cmd, t_args *params, t_cmd_info *cmd_list)
{
    if (!cmd) {
        printf("run_cmd: cmd is NULL\n");
        exit(127);
    }
    fprintf(stderr, "Running command type: %d\n", cmd->type);
	// create linked list there and fill it recursively
	// ATTENTION:  it will not work until it gets only one structure
    if (cmd->type == EXEC)
        cmd_list = fill_exec(cmd, params, cmd_list);
    else if (cmd->type == REDIR)
        cmd_list = fill_redir(cmd, params, cmd_list);
    else if (cmd->type == PIPE)
        cmd_list = fill_pipe(cmd, params, cmd_list);
    else {
        printf("run_cmd: Unknown command type\n");
        exit(127);
    }
	return ;
}

t_cmd_info *fill_pipe(t_cmd *cmd, t_cmd *params, t_cmd_info *new_cmd)
{
	t_pipe	*pcmd;
	pcmd = (t_pipe *)cmd;

	// FT_MEMSET FOR new_cmd!
	new_cmd->type = PIPE;
	new_cmd->argv = pcmd->argv;
	new_cmd->eargv = pcmd->eargv;
	if (pcmd->left->type == PIPE)
	{
		// не записывать в лист, а отправить в рекурсию
		gothrough_cmd(pcmd->left, params, new_cmd);
	}
	else
		add_cmd_to_list(new_cmd);
	if (pcmd->right->type == PIPE)
		gothrough_cmd(pcmd->right, params, new_cmd);
	else
		add_cmd_to_list(new_cmd);
}

t_cmd_info *fill_redir(t_cmd *cmd, t_cmd *params, t_cmd_info *new_cmd)
{
	t_redir *rcmd;

	rcmd = (t_redir *)cmd;
	// FT_MEMSET FOR new_cmd!
	new_cmd->type = REDIR;
	new_cmd->argv = rcmd->argv;
	new_cmd->eargv = rcmd->eargv;
	// add this subcmd there
	new_cmd->subcmd = rcmd->cmd;
	new_cmd->fd_read = rcmd->fd_read;
	new_cmd->fd_write = rcmd->fd_write;
	new_cmd->file_read = rcmd->file_read;
	new_cmd->file_write = rcmd->file_write;
	//add smth else??
	add_cmd_to_list(new_cmd);
}

t_cmd_info *fill_exec(t_cmd *cmd, t_cmd *params, t_cmd_info *new_cmd)
{
	t_execcmd	*ecmd;

	ecmd = (t_execcmd *)cmd;
	check_arguments(ecmd);
	// FT_MEMSET FOR new_cmd!
	new_cmd->type = EXEC;
	new_cmd->argv = ecmd->argv;
	new_cmd->eargv = ecmd->eargv;
	new_cmd->fd_read = 0; //change if redirs
	new_cmd->fd_write = 1; //change if redirs
	// if (!new_cmd->head) //if it's the first cmd, it becomes head.
	// 	new_cmd->head = 1;
	// new_cmd->connection //create a pipe for connectiona and fill it smwh else
	add_cmd_to_list(new_cmd);
}

t_cmd_info *add_cmd_to_list(t_cmd_info *cmd) // it returns head
{
	// TODO
	t_cmd_info	*current;
	t_cmd_info	*new_cmd;
	t_cmd_info	*head;

	new_cmd = cmd;
	new_cmd->next = NULL;
	if (!cmd->head)
	{
		head = new_cmd;
		head->next = NULL;
		cmd->head = 1;
	}
	else
	{
		current = head;
		while (current->next != NULL) // add to the last node
			current = current->next;
		current->next = new_cmd;
	}
	return (head);
}