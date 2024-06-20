/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 17:20:39 by sabdulki          #+#    #+#             */
/*   Updated: 2024/06/20 19:35:24 by sabdulki         ###   ########.fr       */
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
        printf("run_cmd: cmd is NULL\n");
		return ;
	}
    fprintf(stderr, "Running command type: %d\n", cmd->type);
	// create linked list there and fill it recursively
	// ATTENTION:  it will not work until it gets only one structure
    if (cmd->type == EXEC) //builtin or any usual cmd
        cmd_node = fill_exec(cmd);
    else if (cmd->type == REDIR)
        cmd_node = fill_redir(cmd, cmd_list, args);
    else if (cmd->type == PIPE)
        fill_pipe(cmd, cmd_list, args);
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
		gothrough_cmd(pcmd->left, cmd_list, args);
	// }
	// else if (pcmd->right->type == PIPE)
		gothrough_cmd(pcmd->right, cmd_list, args);
	// return (new_cmd);
}

t_cmd_info	*fill_redir(t_cmd *cmd, t_cmd_info **cmd_list, t_args *args)
{
	t_redir		*rcmd;
	t_cmd_info	*new_cmd;

	rcmd = (t_redir *)cmd; 
	new_cmd = malloc(sizeof(t_cmd_info));
	if (!new_cmd)
		return (NULL);
	// FT_MEMSET FOR new_cmd!
	new_cmd->type = rcmd->type;
	new_cmd->subcmd = rcmd->cmd;
	copy_argv(new_cmd, new_cmd->subcmd);
	copy_eargv(new_cmd, new_cmd->subcmd);
	new_cmd->file_read = NULL;
	new_cmd->file_write = NULL;
	new_cmd->fd_read = 0;
	new_cmd->fd_write = 1;
	if (add_redir_details(new_cmd, rcmd, args))
		return (NULL);
	printf("new_cmd->subcmd->type: %d\n", new_cmd->subcmd->type);
	if (new_cmd->subcmd->type == PIPE)
		gothrough_cmd(new_cmd->subcmd, cmd_list, args);
	//it should smth like recursive, because there culd be more than 2 redirs!
	if (new_cmd->subcmd->type == REDIR) 
	{
		more_redir(new_cmd, rcmd, args);
	}
		// return (fill_redir(new_cmd->subcmd, cmd_list));
	return (new_cmd);
}

int	more_redir(t_cmd_info *new_cmd, t_redir *rcmd, t_args *args)
{
	int			type;
	char		*file;
	int			fd;
	t_redir		*rsubcmd;
	t_redir		*tmp;

	(void)new_cmd;
	rsubcmd = (t_redir *)rcmd->cmd;
	type = REDIR;
	while (type == REDIR && rsubcmd->subtype != APPEND)
	{
		if (rsubcmd->subtype == HEREDOC)
		{
			file = heredoc_get_tmp_file();
			heredoc(rsubcmd->fd, file, rsubcmd->file, rsubcmd->mode, args);
		}
		else
		{
			fd = get_file_fd(rsubcmd->fd, rsubcmd->file, rsubcmd->mode); //creates a file
			close (fd);
		}
		type = rsubcmd->cmd->type;
		if (type == REDIR)
		{
			tmp = (t_redir *)rsubcmd->cmd;
			rsubcmd = tmp;
		}
	}
	return (0);
}

int		add_redir_details(t_cmd_info *new_cmd, t_redir *rcmd, t_args *args)
{
	new_cmd->redir_type = rcmd->subtype;
	define_file(new_cmd, rcmd);
	if (define_fd(new_cmd, rcmd, args))
		return (1);
	return (0);
}

int	define_file(t_cmd_info	*rcmd, t_redir *old_cmd)
{
	// rcmd->mode_read = 0 ???
	// rcmd->mode_write = 0 ???
	if (rcmd->redir_type == REDIRIN)
	{
		rcmd->file_read = old_cmd->file;
		rcmd->mode_read = old_cmd->mode;
	}
	if (rcmd->redir_type == REDIROUT)
	{
		rcmd->file_write = old_cmd->file;
		rcmd->mode_write = old_cmd->mode;
	}
	if (rcmd->redir_type == HEREDOC)
	{
		rcmd->file_read = heredoc_get_tmp_file();
		if (!rcmd->file_read)
			return (printf("failed get heredoc tmp file\n"), 1);
		rcmd->mode_read = old_cmd->mode;
	}
	if (rcmd->redir_type == APPEND)
	{
		rcmd->file_write = old_cmd->file;
		rcmd->mode_write = old_cmd->mode;
	}
	return (0);
}

int	define_fd(t_cmd_info *rcmd, t_redir *old_cmd, t_args *args)
{
	if (rcmd->redir_type == REDIRIN)
	{
		rcmd->fd_read = get_file_fd(rcmd->fd_read, rcmd->file_read, rcmd->mode_read); //open and check if it's -1
		if (rcmd->fd_read == -1)
			return (1);
	}
	if (rcmd->redir_type == REDIROUT)
	{
		rcmd->fd_write = get_file_fd(rcmd->fd_write, rcmd->file_write, rcmd->mode_write);
		if (rcmd->fd_write == -1)
			return (1);
	}
	if (rcmd->redir_type == HEREDOC)
		rcmd->fd_read = heredoc(rcmd->fd_read, rcmd->file_read, old_cmd->file, rcmd->mode_read, args) ; // call heredoc function HERE, not in parsing. TODO
	if (rcmd->redir_type == APPEND)
	{
		rcmd->fd_write = get_file_fd(rcmd->fd_write, rcmd->file_write, rcmd->mode_write);
		if (rcmd->fd_write == -1)
			return (1);
	}
	return (0);
}

int	get_file_fd(int fd, char *file, int mode)
{
	int	new_fd;

	new_fd = open(file, mode, 0644); //the permissions for each redir are different!!!! maybe??
	if (new_fd < 0)
	{
		printf("open '%s' failed in get_file_fd\n", file);
		return (-1);
	}
	if (new_fd != fd && fd != 0 && fd != 1) // Close the old file descriptor if they are different
		close(fd); // what about closing 0 or 1 fds???
	return (new_fd);
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
