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

int	 count_files(t_redir *rcmd, int redir_type)
{
	int			type;
	int			counter;
	t_redir		*rsubcmd;
	t_redir		*tmp;

	counter = 0;
	if (rcmd->cmd->type == REDIR)
	{
		rsubcmd = (t_redir *)rcmd->cmd;
		type = REDIR;
		while (type == REDIR)
		{
			// printf("rsubcmd->file: %s\n", rsubcmd->file);
			if (rsubcmd->subtype == redir_type) // HEREDOC, REDIRIN, REDIROUT
				counter += 1;
			type = rsubcmd->cmd->type;
			if (type == REDIR)
			{
				tmp = (t_redir *)rsubcmd->cmd;
				rsubcmd = tmp;
			}
		}
	}
	counter += 1;
	return (counter);
}

char	**create_file_array(t_redir *rcmd, int redir_type)
{
	char	**hfile_arr;
	int			size;

	hfile_arr = NULL;
	size = count_files(rcmd, redir_type);
	if (size > 1)
		hfile_arr = malloc(sizeof(char *) * (size + 1));
	if (!hfile_arr)
		return (NULL);
	return (hfile_arr);
}

int	*create_fd_array(t_redir *rcmd, int redir_type)
{
	int			*fd_arr;
	int			size;

	fd_arr = NULL;
	size = count_files(rcmd, redir_type);
	size += 1;
	// printf("size: %d\n", size);
	if (size > 1)
		fd_arr = malloc(sizeof(int) * (size));
	if (!fd_arr)
		return (NULL);
	return (fd_arr);
}

int	fill_fd_arr(int redir_type, int *arr, t_cmd_info *new_cmd, int	counter)
{
	if (!arr)
		return (-1);
	if (redir_type == REDIRIN && arr)
	{
		arr[counter] = new_cmd->fd_read;
		// fprintf(stderr, "fd in fill: '%d'\n", new_cmd->fd_write);
	}
	else if (redir_type == REDIROUT && arr)
	{
		arr[counter] = new_cmd->fd_write;
	}
	else if (redir_type == APPEND && arr)
	{
		arr[counter] = new_cmd->fd_write;
	}
	counter++;
	return (counter);
}

int	return_last_fd(int *arr, int size) //and free
{
	// int size;
	int	i;
	int	fd;

	i = 0;
	// while (arr[size])
	// 	size++;
	while(i != size)
	{
		close(arr[i]);
		i++;
	}
	fd = arr[i];
	free(arr);
	return (fd); // do not close!
}

char *return_last_file(char **arr)
{
	int size;
	int	i;
	char *file;

	size = 0;
	i = 0;
	while (arr[size])
		size++;
	while(i != size)
	{
		unlink(arr[i]);
		free(arr[i]);
		i++;
	}
	file = arr[i];
	free(arr);
	return (file);
}

int	more_redir(t_cmd_info *new_cmd, t_redir *rcmd, t_args *args)
{
	t_redir		*rsubcmd;
	t_redir		*tmp;
	int			type;
	int			i_hd;
	int			fd;
	char		**heredoc_arr;
	char		**limiter_arr;

	i_hd = 0;
	type = REDIR;
	rsubcmd = (t_redir *)rcmd->cmd;
	heredoc_arr = create_file_array(rcmd, HEREDOC);
	limiter_arr = create_file_array(rcmd, HEREDOC);
	if (heredoc_arr)
	{
		heredoc_arr[i_hd] = new_cmd->file_read;
		limiter_arr[i_hd] = rcmd->file;
		// printf("index: %d, '%s', limiter: '%s'\n", i_hd, heredoc_arr[i_hd], limiter_arr[i_hd]);
		i_hd += 1;
	}
	while (type == REDIR)
	{
		if (rsubcmd->subtype == HEREDOC)
		{
			new_cmd->file_read = heredoc_get_tmp_file();
			if (heredoc_arr)
			{
				heredoc_arr[i_hd] = new_cmd->file_read;
				limiter_arr[i_hd] = rsubcmd->file;
				// printf("index: %d, '%s', limiter: '%s'\n", i_hd, heredoc_arr[i_hd], limiter_arr[i_hd]);
			}
			i_hd += 1;
		}
		else //creates a file
		{
			fd = get_file_fd(rsubcmd->fd, rsubcmd->file, rsubcmd->mode, rsubcmd->subtype);
			close (fd);
			if (fd == -2)
				return (1);
		}
		type = rsubcmd->cmd->type;
		if (type == REDIR)
		{
			tmp = (t_redir *)rsubcmd->cmd;
			free(rsubcmd);
			rsubcmd = tmp;
		}
	}
	if (heredoc_arr)
	{
		heredoc_arr[i_hd] = NULL;
		limiter_arr[i_hd] = NULL;
		if (call_heredocs(heredoc_arr, new_cmd, limiter_arr, args))
			return (1);
	}
	new_cmd->subcmd = rsubcmd->cmd;
	return (0);
}

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
	/* if fd == -1, i have to bring the command to execution with invalid fd.
	and exit with status 1 in child proc, if fd of command == -1 */ 
	if (rcmd->redir_type == REDIRIN)
	{
		rcmd->fd_read = get_file_fd(rcmd->fd_read, rcmd->file_read, rcmd->mode_read, rcmd->redir_type); //open and check if it's -1
	}
	if (rcmd->redir_type == REDIROUT)
	{
		rcmd->fd_write = get_file_fd(rcmd->fd_write, rcmd->file_write, rcmd->mode_write, rcmd->redir_type);
	}
	if (rcmd->redir_type == HEREDOC)
	{
		rcmd->fd_read = heredoc(rcmd->fd_read, rcmd->file_read, old_cmd->file, rcmd->mode_read, args);
		rcmd->fd_read = get_file_fd(rcmd->fd_read, rcmd->file_read, rcmd->mode_read, rcmd->redir_type);
	}
	if (rcmd->redir_type == APPEND)
	{
		rcmd->fd_write = get_file_fd(rcmd->fd_write, rcmd->file_write, rcmd->mode_write, rcmd->redir_type);
	}
	if (rcmd->fd_read == -1 || rcmd->fd_write == -1)
		return (1);
	return (0);
}

int	get_file_fd(int fd, char *file, int mode, int redir_type)
{
	int	new_fd;

	new_fd = open(file, mode, 0777); //the permissions for each redir are different!!!! maybe??
	if (check_file_access(file, redir_type) != 0)
		return (printf("bash: %s: Permission denied\n", file), -1);
	if (new_fd < 0)
	{
		printf("open '%s' failed\n", file);
		return (-1);
	}
	if (new_fd != fd && fd != 0 && fd != 1) // Close the old file descriptor if they are different
		close(fd);
	return (new_fd);
}