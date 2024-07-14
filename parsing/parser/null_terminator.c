/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   null_terminator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlomakin <vlomakin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 15:31:59 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/14 16:29:36 by vlomakin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	nulterminate_exec(t_execcmd *execcmd)
{
	int	i;

	i = 0;
	while (execcmd->argv[i])
	{
		if (execcmd->eargv[i])
		{
			*execcmd->eargv[i] = 0;
		}
		else
		{
			printf("nulterminate_exec: eargv[%d] is NULL\n", i);
		}
		i++;
	}
}

void	nulterminate_pipe(t_pipe *pipecmd)
{
	if (!pipecmd->left)
	{
		printf("nulterminate_pipe: left is NULL\n");
	}
	else
	{
		nulterminate(pipecmd->left);
	}
	if (!pipecmd->right)
	{
		printf("nulterminate_pipe: right is NULL\n");
	}
	else
	{
		nulterminate(pipecmd->right);
	}
}

void	nulterminate_redir(t_redir *redircmd)
{
	*redircmd->efile = 0;
	nulterminate(redircmd->cmd);
}

t_cmd	*nulterminate(t_cmd *cmd)
{
	if (cmd == 0)
		return (0);
	if (cmd->type == EXEC)
		nulterminate_exec((t_execcmd *)cmd);
	else if (cmd->type == PIPE)
		nulterminate_pipe((t_pipe *)cmd);
	else if (cmd->type == REDIR)
		nulterminate_redir((t_redir *)cmd);
	return (cmd);
}
