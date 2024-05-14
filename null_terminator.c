#include "minishell.h"

void nulterminate_exec(t_execcmd *execcmd) 
{
    int i;
    
    i = 0;
    while (execcmd->argv[i]) 
    {
        *execcmd->eargv[i] = 0;
        i++;
    }
}

void nulterminate_pipe(t_pipe *pipecmd) 
{
    nulterminate(pipecmd->left);
    nulterminate(pipecmd->right);
}

void nulterminate_redir(t_redir *redircmd) 
{
    nulterminate(redircmd->cmd);
    *redircmd->efile = 0;
}

t_cmd *nulterminate(t_cmd *cmd) 
{
    if (cmd == 0)
        return 0;
    if (cmd->type == EXEC) 
        nulterminate_exec((t_execcmd *)cmd);
    else if (cmd->type == PIPE)
        nulterminate_pipe((t_pipe *)cmd);
    else if (cmd->type == REDIR)
        nulterminate_redir((t_redir *)cmd);
    return (cmd);
}
