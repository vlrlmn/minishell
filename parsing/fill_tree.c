/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:03:40 by vlomakin          #+#    #+#             */
/*   Updated: 2024/06/03 19:52:57 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*pipecmd(t_cmd *left, t_cmd *right)
{
	t_pipe	*cmd;
	int i = 0;
	t_execcmd *ecmd;

	printf("Pipecmd\n");
	cmd = (t_pipe*)malloc(sizeof(t_pipe));
	if (!cmd)
		exit_with_malloc_error(MALLOC_ERROR);
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	ecmd = (t_execcmd*)cmd->left;
	printf("Left %d\n", ecmd->type);
	i=0;
	while(ecmd->argv[i])
	{
		printf("Arg %d: %.*s\n", i, (int)(ecmd->eargv[i] - ecmd->argv[i]), ecmd->argv[i]);
		i++;
	}
	
	ecmd = (t_execcmd*)cmd->right;
	printf("Right %d\n", ecmd->type);
	i=0;
	while(ecmd->argv[i])
	{
		printf("Arg %d: %.*s\n", i, (int)(ecmd->eargv[i] - ecmd->argv[i]), ecmd->argv[i]);
		i++;
	}

	return ((t_cmd *)cmd);
}

