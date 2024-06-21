/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 14:22:40 by sabdulki          #+#    #+#             */
/*   Updated: 2024/06/21 14:29:24 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	redir(t_redir *rcmd)
{
	int		new_fd;

	new_fd = open(rcmd->file, rcmd->mode, 0644);
	if (new_fd < 0) //Implicit File Descriptor Assignment
	{
		printf("open '%s' failed in redir\n", rcmd->file);
		exit(126);
	}
	if (new_fd != rcmd->fd) // Close the old file descriptor if they are different
		close(rcmd->fd);
	rcmd->fd = new_fd;
}
