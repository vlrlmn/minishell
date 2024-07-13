/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 14:22:40 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/13 15:10:47 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int		add_redir_details(t_cmd_info *new_cmd, t_redir *rcmd, t_args *args)
{
	new_cmd->redir_type = rcmd->subtype;
	define_file(new_cmd, rcmd);
	if (count_files(rcmd, HEREDOC) == 1 || new_cmd->redir_type != HEREDOC)
	{
		if (define_fd(new_cmd, rcmd, args))
			return (1);
	}
	return (0);
}

int	define_file(t_cmd_info	*rcmd, t_redir *old_cmd)
{
	if (rcmd->redir_type == REDIRIN)
	{
		rcmd->file_read = old_cmd->file;
		rcmd->mode_read = old_cmd->mode;
	}
	if (rcmd->redir_type == REDIROUT || rcmd->redir_type == APPEND)
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
	return (0);
}

int	define_fd(t_cmd_info *rcmd, t_redir *old_cmd, t_args *args)
{
	/* if fd == -1, i have to bring the command to execution with invalid fd.
	and exit with status 1 in child proc, if fd of command == -1 */ 
	if (rcmd->redir_type == REDIRIN)
	{
		rcmd->fd_read = get_file_fd(rcmd, rcmd->redir_type);
	}
	if (rcmd->redir_type == REDIROUT || rcmd->redir_type == APPEND)
	{
		rcmd->fd_write = get_file_fd(rcmd, rcmd->redir_type);
	}
	if (rcmd->redir_type == HEREDOC)
	{
		rcmd->fd_read = heredoc(rcmd, old_cmd->file, args);
		if (rcmd->fd_read == -2)
		{
			unlink(rcmd->file_read);
			free(rcmd->file_read);	
			rcmd->file_read = NULL;
			return (1);
		}
		rcmd->fd_read = get_file_fd(rcmd, rcmd->redir_type);
	}
	// if (rcmd->fd_read == -1 || rcmd->fd_write == -1)
	// 	return (1);
	return (0);
}

int	get_file_fd(t_cmd_info* cmd, int redir_type)
{
	int	new_fd;
	int fd;
	char *file;
	int mode;

	file = get_file(cmd);
	fd = get_fd_or_mode(cmd, 'f');
	mode = get_fd_or_mode(cmd, 'm');
	new_fd = open(file, mode, 0777); 
	if (check_file_access(file, redir_type) != 0)
		return (-1);
	if (new_fd < 0)
		return (-1);
	if (new_fd != fd && fd != 0 && fd != 1)
		close(fd);
	return (new_fd);
}
