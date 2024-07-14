/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 20:02:39 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/13 20:02:58 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_argv(t_cmd_info *current)
{
	int	i;

	i = 0;
	if (!current->argv[i])
		return ;
	while (current->argv[i])
	{
		free(current->argv[i]);
		current->argv[i] = NULL;
		current->eargv[i] = NULL;
		i++;
	}
}

void	free_files_and_fd(t_cmd_info *current)
{
	if (current->fd_read != 0 && current->fd_read != 1)
		close(current->fd_read);
	if (current->fd_write != 0 && current->fd_write != 1)
		close(current->fd_write);
	if (current->file_read && current->redir_type == HEREDOC)
	{
		unlink(current->file_read);
		free(current->file_read);
	}
}
