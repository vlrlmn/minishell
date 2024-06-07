/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 17:36:36 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/06/07 12:26:32 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void exec_lexer(t_cmd *cmd, t_args *args)
{
    
}

void check_file()
{

}

void redir_lexer(t_cmd *cmd, t_args *args)
{
    t_redir *redircmd;

    redircmd = (t_redir *)cmd;
    if (redircmd->type == '-')
        check_file(); // check if type is saved in parsing
    lexical_analysis(redircmd->cmd, args);
}

void    lexical_analysis(t_cmd *cmd, t_args *args)
{
    t_pipe *pipecmd;
    
    if(cmd->type == PIPE)
    {
        pipecmd = (t_pipe *)cmd;
        lexical_analysis(pipecmd->left, args);
        lexical_analysis(pipecmd->right, args);
    }
    else if (cmd->type == REDIR)
        redir_lexer(cmd, args);
    else if (cmd->type == EXEC)
        exec_lexer(cmd, args);
}