/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlomakin <vlomakin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:03:40 by vlomakin          #+#    #+#             */
/*   Updated: 2024/05/15 14:08:55 by vlomakin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pipe	*pipecmd(t_cmd *left, t_cmd *right)
{
	t_pipe	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		exit_with_malloc_error(MALLOC_ERROR);
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((t_cmd *)cmd);
}

