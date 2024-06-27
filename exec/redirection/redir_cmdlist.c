/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:23:17 by sabdulki          #+#    #+#             */
/*   Updated: 2024/06/12 20:24:33 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/* When you use >>, 
Bash opens the specified file in append mode and writes the output of the command to it. 
If the file does not exist, it will be created. */
// int	append()
// {

// }

int	count_heredoc_files(t_redir *rcmd)
{
	int			type;
	int			counter;
	t_redir		*rsubcmd;
	t_redir		*tmp;

	counter = 0;
	rsubcmd = (t_redir *)rcmd->cmd;
	type = REDIR;
	while(type == REDIR)
	{
		if (rsubcmd->subtype == HEREDOC)
			counter +=1;
		type = rsubcmd->cmd->type;
		if (type == REDIR)
		{
			tmp = (t_redir *)rsubcmd->cmd;
			rsubcmd = tmp;
		}
	}
	return (counter);
}

char	**create_heredoc_file_array(t_redir *rcmd)
{
	char	**hfile_arr;
	int			size;

	size = count_heredoc_files(rcmd);
	hfile_arr = malloc(sizeof(char *) * (size + 1));
	if (!hfile_arr)
		return (NULL);
	return (hfile_arr);
}

int	more_redir(t_cmd_info *new_cmd, t_redir *rcmd, t_args *args)
{
	int			type;
	int			counter;
	char		*file;
	int			fd;
	t_redir		*rsubcmd;
	t_redir		*tmp;
	
	char		**hfile_arr;

	counter = 0;
	rsubcmd = (t_redir *)rcmd->cmd;
	type = REDIR;

	hfile_arr = create_heredoc_file_array(rcmd);
	while (type == REDIR && rsubcmd->subtype != APPEND)
	{
		if (rsubcmd->subtype == HEREDOC)
		{
			file = heredoc_get_tmp_file(); //write this file to some struct to unlink and free it later
			heredoc(rsubcmd->fd, file, rsubcmd->file, rsubcmd->mode, args);
			if (hfile_arr)
				hfile_arr[counter] = file;
			counter +=1;
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
	if (hfile_arr)
		hfile_arr[counter] = NULL;
	new_cmd->hfile_array = hfile_arr;
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
	/* if fd == -1, i have to bring the command to execution with invalis fd.
	and exit wit status 1 in child proc, if fd od command == -1 */ 
	if (rcmd->redir_type == REDIRIN)
	{
		rcmd->fd_read = get_file_fd(rcmd->fd_read, rcmd->file_read, rcmd->mode_read); //open and check if it's -1
	}
	if (rcmd->redir_type == REDIROUT)
	{
		rcmd->fd_write = get_file_fd(rcmd->fd_write, rcmd->file_write, rcmd->mode_write);
	}
	if (rcmd->redir_type == HEREDOC)
	{
		rcmd->fd_read = heredoc(rcmd->fd_read, rcmd->file_read, old_cmd->file, rcmd->mode_read, args) ; // call heredoc function HERE, not in parsing. TODO
		rcmd->fd_read = get_file_fd(rcmd->fd_read, rcmd->file_read, rcmd->mode_read);
	}
	if (rcmd->redir_type == APPEND)
	{
		rcmd->fd_write = get_file_fd(rcmd->fd_write, rcmd->file_write, rcmd->mode_write);
		// if (append(rcmd->fd_write, rcmd->argv, args))
		// 	return (1);
		// rcmd->fd_write = get_file_fd(rcmd->fd_write, rcmd->file_write, rcmd->mode_write);
	}
	return (0);
}

int	get_file_fd(int fd, char *file, int mode)
{
	int	new_fd;

	// if (check_file_access(file, R_OK))
	// 	return (-1);
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