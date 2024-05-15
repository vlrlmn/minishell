/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aajaanan <aajaanan@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 10:44:47 by aajaanan          #+#    #+#             */
/*   Updated: 2023/09/01 19:47:58 by aajaanan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

void	run_cmd(t_cmd *cmd, t_params *params, int *exit_status)
{
	if (cmd->type == PIPE)
		run_pipe(cmd, params, exit_status);
	else if (cmd->type == REDIR)
		run_redir(cmd, params, exit_status);
	else if (cmd->type == EXEC)
		run_exec(cmd, params, exit_status);
	exit(*exit_status);
}
