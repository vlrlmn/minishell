/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 17:36:36 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/06/06 17:58:44 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    lexical_analysis(t_cmd *cmd, t_args *args)
{
    t_pipe *pipecmd;
    t_redir *redircmd;
    
    if(cmd->type == PIPE)
    {
        pipecmd = (t_pipe *)cmd;
        lexical_analysis(pipecmd->left, args);
        lexical_analysis(pipecmd->right, args);
    }
    else if (cmd->type == REDIR)
    {
        redircmd = (t_redir *)cmd;
        lexical_analysis(redircmd->cmd, args);
    }
    else if (cmd->type == EXEC)
        exec_lexer(cmd, args);
}