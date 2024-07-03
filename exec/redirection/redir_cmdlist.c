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
	printf("size: %d\n", size);
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
	int			type;
	// int			i_in;
	// int			i_out;
	// int			i_app;
	int			i_hd;
	t_redir		*rsubcmd;
	t_redir		*tmp;
	// char		*limiter;
	// char		*file;

	char		**heredoc_arr;
	char		**limiter_arr;
	// int			*redirin_arr;
	// int			*redirout_arr;
	// int			*append_arr;

	// i_in = 0;
	// i_out = 0;
	// i_app = 0;
	i_hd = 0;
	rsubcmd = (t_redir *)rcmd->cmd;
	type = REDIR;
	int	fd;

	heredoc_arr = create_file_array(rcmd, HEREDOC); //0
	limiter_arr = create_file_array(rcmd, HEREDOC); //0


	// redirin_arr = create_fd_array(rcmd, REDIRIN); //2
	// redirout_arr = create_fd_array(rcmd, REDIROUT); //0
	// append_arr = create_fd_array(rcmd, APPEND); //0

	/* every time I can write fd and filename 
	to the new_cmd->field. If i have more than 1 redir,
	it would be rewrited */
	/* for heredoc I have to save file
	For redirin, redirout and append i have to save FD! */
	// i_in = fill_fd_arr(rsubcmd->subtype, redirin_arr, new_cmd, i_in);
	// i_out = fill_fd_arr(rsubcmd->subtype, redirout_arr, new_cmd, i_out);
	// i_app = fill_fd_arr(rsubcmd->subtype, append_arr, new_cmd, i_app);
	// if (rsubcmd->subtype == HEREDOC)
	// {
	// 	if (heredoc_arr)
	// 		heredoc_arr[i_hd] = new_cmd->file_read;
	// 	i_hd += 1;
	// }
	if (heredoc_arr)
	{
		heredoc_arr[i_hd] = new_cmd->file_read;
		limiter_arr[i_hd] = rcmd->file;
		printf("index: %d, '%s', limiter: '%s'\n", i_hd, heredoc_arr[i_hd], limiter_arr[i_hd]);
		i_hd += 1;
	}
	while (type == REDIR) // && rsubcmd->subtype != APPEND)
	{
		// fprintf(stderr, "file: '%s'\n", new_cmd->file_write);
		// fprintf(stderr, "fd: '%d'\n", new_cmd->fd_write);
		// add_redir_details(new_cmd, rsubcmd, args);

		// i_in = fill_fd_arr(rsubcmd->subtype, redirin_arr, new_cmd, i_in);
		// i_out = fill_fd_arr(rsubcmd->subtype, redirout_arr, new_cmd, i_out);
		// i_app = fill_fd_arr(rsubcmd->subtype, append_arr, new_cmd, i_app);
		if (rsubcmd->subtype == HEREDOC)
		{
			new_cmd->file_read = heredoc_get_tmp_file(); //write this file to some struct to unlink and free it later
			// heredoc(rsubcmd->fd, file, rsubcmd->file, rsubcmd->mode, args);
			if (heredoc_arr)
			{
				heredoc_arr[i_hd] = new_cmd->file_read;
				limiter_arr[i_hd] = rsubcmd->file;
				printf("index: %d, '%s', limiter: '%s'\n", i_hd, heredoc_arr[i_hd], limiter_arr[i_hd]);
			}
			i_hd += 1;
		}
		else
		{
			fd = get_file_fd(rsubcmd->fd, rsubcmd->file, rsubcmd->mode); //creates a file
			// check file access
			close (fd);
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

		// print heredoc array
		// call heredoc() func from h1 to h-n file;
		call_heredocs(heredoc_arr, new_cmd, limiter_arr, args);
		// new_cmd->file_read = h3;
		// new_cmd->fd_read = get_file_fd();
		// new_cmd->file_read = return_last_file(heredoc_arr);
	}
	// printf("i_in: %d\n", i_in);
	// if (redirin_arr)
	// 	new_cmd->fd_read = return_last_fd(redirin_arr, i_in);
	// if (redirout_arr)
	// 	new_cmd->fd_write = return_last_fd(redirout_arr, i_out);
	// if (append_arr)
	// 	new_cmd->fd_write = return_last_fd(append_arr, i_app);
	new_cmd->subcmd = rsubcmd->cmd;
	return (0);
}

int		add_redir_details(t_cmd_info *new_cmd, t_redir *rcmd, t_args *args)
{
	new_cmd->redir_type = rcmd->subtype;
	define_file(new_cmd, rcmd);
	// if (new_cmd->redir_type != HEREDOC
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
		rcmd->fd_read = heredoc(rcmd->fd_read, rcmd->file_read, old_cmd->file, rcmd->mode_read, args);
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