/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 14:27:29 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/13 20:06:06 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/* modifies head of linked list and addes new cmd_node to the list of cmds. */
int	modify_static_int(t_signal_type flag)
{
	static int	i_list;

	if (flag == SET)
		i_list += 1;
	if (flag == SET_ZERO)
		i_list = 0;
	return (i_list);
}

void	add_cmd_to_list(t_cmd_info *cmd, t_cmd_info **head)
{
	t_cmd_info	*current;
	t_cmd_info	*new_cmd;
	int			i_list;

	i_list = modify_static_int(SET);
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
		while (current->next != NULL)
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

void	free_cmd_list(t_cmd_info *cmd_list)
{
	t_cmd_info	*current;
	t_cmd_info	*tmp;

	if (!cmd_list)
		return ;
	current = cmd_list;
	while (current)
	{
		tmp = current->next;
		free_argv(current);
		free_files_and_fd(current);
		free(current->connection);
		if (current->type == REDIR && current->subcmd)
		{
			if (current->subcmd->type == EXEC)
				free((t_execcmd *)current->subcmd);
		}
		free(current);
		current = tmp;
	}
	modify_static_int(SET_ZERO);
}
