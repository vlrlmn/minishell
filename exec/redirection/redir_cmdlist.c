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
			if (rsubcmd->subtype == redir_type)
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



int	check_files(t_redir *rsubcmd)
{
	int	fd;

	fd = r_get_file_fd(rsubcmd, rsubcmd->subtype);
	close (fd);
	if (fd == -1)
		return (free(rsubcmd), free((t_execcmd *)rsubcmd->cmd), 1);
	return (0);
}

t_redir	*go_next_rsubcmd(t_redir *rsubcmd)
{
	t_redir		*tmp;

	tmp = (t_redir *)rsubcmd->cmd;
	free(rsubcmd);
	rsubcmd = tmp;
	return (rsubcmd);
}

t_redir *while_redir(t_redir *rsubcmd, char **heredoc_arr, t_cmd_info *new_cmd, int i)
{
	int			type;

	type = REDIR;
	while (type == REDIR)
	{
		if (rsubcmd->subtype == HEREDOC)
		{
			new_cmd->file_read = heredoc_get_tmp_file();
			i = fill_heredoc_array(heredoc_arr, i, new_cmd, rsubcmd);
		}
		else
		{
			if (check_files(rsubcmd))
				return (NULL);
		}
		type = rsubcmd->cmd->type;
		if (type == REDIR)
			rsubcmd = go_next_rsubcmd(rsubcmd);
	}
	if (heredoc_arr)
		heredoc_arr[i] = NULL;
	return (rsubcmd);
}

int	more_redir(t_cmd_info *new_cmd, t_redir *rcmd, t_args *args)
{
	t_redir		*rsubcmd;
	char		**heredoc_arr;
	int			i;

	i = 0;
	rsubcmd = (t_redir *)rcmd->cmd;
	heredoc_arr = create_file_array(rcmd, HEREDOC);
	i = fill_heredoc_array(heredoc_arr, i, new_cmd, rcmd);
	rsubcmd = while_redir(rsubcmd, heredoc_arr, new_cmd, i);
	if (!rsubcmd)
		return (1);
	if (heredoc_arr)
	{
		if (call_heredocs(heredoc_arr, new_cmd, args))
		{
			if (rsubcmd->cmd->type == 1)
				free((t_execcmd *)rsubcmd->cmd);
			return (1);
		}
	}
	new_cmd->subcmd = rsubcmd->cmd;
	return (free(rsubcmd), 0);
}
