/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 17:36:36 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/06/09 15:39:28 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void lexical_analysis(t_cmd *cmd, t_args *args)
{
    t_execcmd *exec;
    t_redir *rcmd;
    t_pipe *pcmd;
    int i;

    if (cmd->type == EXEC)
    {
        exec = (t_execcmd *)cmd;
        while (exec->argv[i])
        {
            if (is_parse_symbol(exec->argv[i]))
                exec->argv[i] = clean_cmd(exec->argv[i], args);
            i++;
        }
    }
    else if (cmd->type == REDIR)
    {
        rcmd = (t_redir *)cmd;
        if (rcmd->type == '-')
            rcmd->file = clean_file(rcmd->file, args);
        lexical_analysis(rcmd->cmd, args);
    }
    else if (cmd->type == PIPE)
    {
        pcmd = (t_pipe *)cmd;
        lexical_analysis(pcmd->left, args);
        lexical_analysis(pcmd->right, args);
    }
}
