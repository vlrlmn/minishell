/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 17:36:36 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/06/08 13:53:26 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../minishell.h"

void lexical_analysis(t_cmd *cmd, t_args *args);

static void process_quotes_inplace(char *token)
{
}

static void clean_quotes(t_cmd *cmd)
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
            process_quotes_inplace(exec->argv[i]);
            i++;
        }
    }
    else if (cmd->type == REDIR)
    {
        rcmd = (t_redir *)cmd;
        clean_quotes(rcmd->cmd);
    }
    else if (cmd->type == PIPE)
    {
        pcmd = (t_pipe *)cmd;
        clean_quotes(pcmd->left);
        clean_quotes(pcmd->right);
    }
}

void lexical_analysis(t_cmd *cmd, t_args *args)
{
    (void)args; // args is not used in this function but might be needed for other purposes

    if (cmd)
        clean_quotes(cmd);
}
