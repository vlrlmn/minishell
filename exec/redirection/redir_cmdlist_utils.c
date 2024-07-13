/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_cmdlist_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 19:56:01 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/13 19:56:29 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

int	fill_heredoc_array(char **heredoc_arr, int i, t_cmd_info *new_cmd, t_redir *rcmd)
{
	if (heredoc_arr)
	{
		heredoc_arr[i] = new_cmd->file_read;
		i++;
		heredoc_arr[i] = rcmd->file;
		i += 1;
	}
	return (i);
}
