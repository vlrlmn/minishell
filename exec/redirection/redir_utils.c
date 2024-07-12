/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 02:39:30 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/12 15:34:27 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char *get_file(t_cmd_info* cmd)
{
	char *file;

	if (cmd->redir_type == REDIRIN || cmd->redir_type == HEREDOC)
		file = cmd->file_read;
	else
		file = cmd->file_write;
	return (file);
}

int	get_fd_or_mode(t_cmd_info* cmd, char flag)
{
	int fd;
	int	mode;
	
	if (flag == 'f')
	{
		if (cmd->redir_type == REDIRIN || cmd->redir_type == HEREDOC)
			fd = cmd->fd_read;
		else
			fd = cmd->fd_write;
		return (fd);
	}
	if (flag == 'm')
	{
		if (cmd->redir_type == REDIRIN || cmd->redir_type == HEREDOC)
			mode = cmd->mode_read;
		else
			mode = cmd->mode_write;
		return (mode);
	}
	else
		return (-1);
	
}
// rsubcmd->fd, rsubcmd->file, rsubcmd->mode, rsubcmd->subtype
int	r_get_file_fd(t_redir *rcmd, int subtype)
{
	int	new_fd;
	int fd;
	char *file;
	int mode;

	file = rcmd->file;
	fd = rcmd->fd;
	mode = rcmd->mode;
	new_fd = open(file, mode, 0777); 
	if (check_file_access(file, subtype) != 0)
		return (printf("bash: %s: Permission denied\n", file), -1);
	if (new_fd < 0)
	{
		printf("open '%s' failed\n", file);
		return (-1);
	}
	if (new_fd != fd && fd != 0 && fd != 1)
		close(fd);
	return (new_fd);
}
