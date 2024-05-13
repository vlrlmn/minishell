#include "minishell.h"

t_cmd *nulterminate(t_cmd *cmd)
{
    int i;
    t_execcmd *execcmd;
    t_pipe *pcmd;
    t_redir *rcmd;

    i = 0;
    if (cmd == 0)
        return (0);
    if(cmd->type == EXEC)
    {
        execcmd = (t_execcmd*)cmd;
        while(execcmd->argv[i])
        {
            *execcmd->eargv[i] = 0;
            i++;
        }
        break;
    }
    else if(cmd->type == PIPE)
    {

    }
    else if(cmd->type == REDIR)
    {

    }
    return(cmd);
}

void parser(t_args *args)
{
    char *es;
    t_cmd *cmd;

    es = args->input + ft_strlen(args->input);
    cmd = parsepipe(&args->input, es);
    peek(&args->input, es, "");
    if(args->input != es)
        exit_with_syntax_err(args, SYNTAX_ERR);
    nulterminate(cmd);
    return (cmd);
}