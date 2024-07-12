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

/* When you use >>, 
Bash opens the specified file in append mode and writes the output of the command to it. 
If the file does not exist, it will be created. */

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
			//function
			new_cmd->file_read = heredoc_get_tmp_file();
			if (heredoc_arr)
			{
				heredoc_arr[i_hd] = new_cmd->file_read;
				limiter_arr[i_hd] = rsubcmd->file;
				// printf("index: %d, '%s', limiter: '%s'\n", i_hd, heredoc_arr[i_hd], limiter_arr[i_hd]);
			}
			//
			i_hd += 1;
		}
		else //creates a file
		{
			fd = r_get_file_fd(rsubcmd, rsubcmd->subtype);
			close (fd);
			if (fd == -1)
				return (free(rsubcmd), 1);
		}
		type = rsubcmd->cmd->type;
		if (type == REDIR)
		{
			tmp = (t_redir *)rsubcmd->cmd;
			free(rsubcmd);
			rsubcmd = tmp;
		}
	}
	//function
	if (heredoc_arr)
	{
		heredoc_arr[i_hd] = NULL;
		limiter_arr[i_hd] = NULL;
		if (call_heredocs(heredoc_arr, new_cmd, limiter_arr, args))
			return (1);
	}
	//
	new_cmd->subcmd = rsubcmd->cmd;
	return (0);
}
