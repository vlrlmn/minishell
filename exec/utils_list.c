/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 14:27:29 by sabdulki          #+#    #+#             */
/*   Updated: 2024/06/20 15:58:29 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
/* modifies head of linked list and addes new cmd_node to the linked list of cmds. */
void	add_cmd_to_list(t_cmd_info *cmd, t_cmd_info	**head)
{
	t_cmd_info	*current;
	t_cmd_info	*new_cmd;

	new_cmd = cmd;
	new_cmd->next = NULL;
	if (!*head)
	{
		*head = new_cmd;
	}
	else
	{
		current = *head;
		while (current->next != NULL) // reach the last node
			current = current->next;
		current->next = new_cmd;
	}
	return ;
}

void	free_cmd_list(t_cmd_info	*cmd_list)
{
	t_cmd_info	*current;
	t_cmd_info	*tmp;

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
		free(current);
		current = tmp;
	}
	return ;
}
