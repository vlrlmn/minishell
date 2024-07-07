/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 14:27:29 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/04 11:30:18 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	i_list;

static int	i_list;

/* modifies head of linked list and addes new cmd_node to the linked list of cmds. */
void	add_cmd_to_list(t_cmd_info *cmd, t_cmd_info	**head)
{
	t_cmd_info	*current;
	t_cmd_info	*new_cmd;

	i_list += 1;
	new_cmd = cmd;
	new_cmd->next = NULL;
	if (!*head)
	{
		*head = new_cmd;
		new_cmd->head = 1;
	}
	else
	{
		current = *head;
		while (current->next != NULL) // reach the last node
			current = current->next;
		new_cmd->head = 0;
		current->next = new_cmd;
	}
	new_cmd->index = i_list;
	return ;
}

int	list_size(t_cmd_info *cmd_list)
{
	t_cmd_info	*current_cmd;
	int			size;

	size = 0;
	current_cmd = cmd_list;
	while (current_cmd)
	{
		size++;
		current_cmd = current_cmd->next;
	}
	return (size);
}

void	free_cmd_list(t_cmd_info	*cmd_list)
{
	t_cmd_info	*current;
	t_cmd_info	*tmp;
	int i;

	if (!cmd_list)
		return ;
	current = cmd_list;
	while (current)
	{
		tmp = current->next;
		// free_cmd(current);
		if (current->fd_read != 0 && current->fd_read != 1)
			close(current->fd_read);
		if (current->fd_write != 0 && current->fd_write != 1)
			close(current->fd_write);
		if (current->file_read && current->redir_type == HEREDOC)
		{
			unlink(current->file_read);
			free(current->file_read);
		}
		//free current->subcmd ?
		free(current->connection);
		if (current->hfile_array)
			free_hfile_arr(current->hfile_array);

		i = 0;
		while (current->argv[i])
		{
			free(current->argv[i]);
			current->argv[i] = NULL;
			current->eargv[i] = NULL;
			i++;
		}

		free(current);
		current = tmp;
	}
	i_list = 0;
	i_list = 0;
	return ;
}

void	free_hfile_arr(char **hfile_array)
{
	int	i;

	i = 0;
	if (!hfile_array)
		return ;
	while (hfile_array[i])
	{
		free(hfile_array[i]);
		i++;
	}
	free(hfile_array);
}