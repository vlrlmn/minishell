/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 02:39:30 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/13 20:35:06 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	define_redir_info(t_cmd_info *new_cmd, t_redir *rcmd)
{
	ft_memset(new_cmd, 0, sizeof(*new_cmd));
	new_cmd->type = rcmd->type;
	new_cmd->subcmd = rcmd->cmd;
	new_cmd->file_read = NULL;
	new_cmd->file_write = NULL;
	new_cmd->fd_read = 0;
	new_cmd->fd_write = 1;
}

void	free_redir(t_redir *rcmd)
{
	t_execcmd	*ecmd;
	int			i;

	i = 0;
	if (rcmd->cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)rcmd->cmd;
		if (!ecmd->argv[i])
		{
			free(ecmd);
			free(rcmd);
			return ;
		}
		while (ecmd->argv[i])
		{
			free(ecmd->argv[i]);
			ecmd->argv[i] = NULL;
			ecmd->eargv[i] = NULL;
			i++;
		}
		free(ecmd);
	}
	else
		free(rcmd->cmd);
	free(rcmd);
}

char	*get_file(t_cmd_info *cmd)
{
	char	*file;

	if (cmd->redir_type == REDIRIN || cmd->redir_type == HEREDOC)
		file = cmd->file_read;
	else
		file = cmd->file_write;
	return (file);
}

int	get_fd_or_mode(t_cmd_info *cmd, char flag)
{
	int	fd;
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

int	r_get_file_fd(t_redir *rcmd, int subtype)
{
	int		new_fd;
	int		fd;
	char	*file;
	int		mode;

	file = rcmd->file;
	fd = rcmd->fd;
	mode = rcmd->mode;
	new_fd = open(file, mode, 0777);
	if (check_file_access(file, subtype) != 0)
		return (printf("bash: %s: Permission denied\n", file), -1);
	if (new_fd < 0)
	{
		return (-1);
	}
	if (new_fd != fd && fd != 0 && fd != 1)
		close(fd);
	return (new_fd);
}
