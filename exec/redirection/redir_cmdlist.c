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
		hfile_arr = malloc(sizeof(char *) * ((size * 2) + 1));
	if (!hfile_arr)
		return (NULL);
	return (hfile_arr);
}

void	print_arr(char **arr)
{
	int i = 0;
	while(arr[i])
	{
		printf("index: %d, str: %s\n", i, arr[i]);
		i++;
	}
}

int	fill_heredoc_array(char **heredoc_arr, int i, t_cmd_info *new_cmd, t_redir *rcmd)
{
	if (heredoc_arr)
	{
		heredoc_arr[i] = new_cmd->file_read;
		i++;
		heredoc_arr[i] = rcmd->file;
		printf("index: %d, '%s', limiter: '%s'\n", i, heredoc_arr[i-1], heredoc_arr[i]);
		i += 1;
	}
	return (i); //alredy incremented
}

int	more_redir(t_cmd_info *new_cmd, t_redir *rcmd, t_args *args)
{
	t_redir		*rsubcmd;
	t_redir		*tmp;
	int			type;
	int			i;
	int			fd;
	char		**heredoc_arr;

	i = 0;
	type = REDIR;
	rsubcmd = (t_redir *)rcmd->cmd;
	heredoc_arr = create_file_array(rcmd, HEREDOC);
	i = fill_heredoc_array(heredoc_arr, i, new_cmd, rcmd); // returns alredy incremented i
	// free(t_execcmd *)rcmd->cmd));
	while (type == REDIR)
	{
		if (rsubcmd->subtype == HEREDOC)
		{
			new_cmd->file_read = heredoc_get_tmp_file();
			i = fill_heredoc_array(heredoc_arr, i, new_cmd, rsubcmd);
		}
		else //creates a file
		{
			fd = r_get_file_fd(rsubcmd, rsubcmd->subtype);
			close (fd);
			if (fd == -1)
				return (free(rsubcmd), 1); //check if not double free
		}
		type = rsubcmd->cmd->type;
		if (type == REDIR)
		{
			tmp = (t_redir *)rsubcmd->cmd;
			// free(rsubcmd->cmd);
			free(rsubcmd); // ??
			rsubcmd = tmp;
		}
		// else
		// 	free(rsubcmd->cmd);
	}
	if (heredoc_arr)
	{
		heredoc_arr[i] = NULL;
		print_arr(heredoc_arr);
		if (call_heredocs(heredoc_arr, new_cmd, args))
		{
			if (type == 1)
				free((t_execcmd *)rsubcmd->cmd);
			return (1);
		}
	}
	new_cmd->subcmd = rsubcmd->cmd;
	return (0);
}
