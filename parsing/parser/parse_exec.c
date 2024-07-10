/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 16:49:26 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/10 14:09:40 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*execcmd(void)
{
	t_execcmd	*exec;

	exec = malloc(sizeof(*exec));
	if (!exec)
		return (NULL);
	ft_memset(exec, 0, sizeof(*exec));
	exec->type = EXEC;
	return ((t_cmd *)exec);
}

t_cmd *initexec(char **ps, char *es) 
{
    t_cmd *cmd;

    cmd = execcmd();
    cmd = parseredir(cmd, ps, es);
    return (cmd);
}

void processargs(t_execcmd *exec, t_cmd **cmd, char **ps, char *es) 
{
    char *q;
    char *eq;
    int tok;
    int argc;
    
    argc = 0;
    while (!peek(ps, es, "|")) 
    {
        if ((tok = gettoken(ps, es, &q, &eq)) == 0)
            break;
        if (tok != 'a')
            printf("syntax");
        exec->argv[argc] = q;
        exec->eargv[argc] = eq;
        argc++;
        if (argc >= MAXARGS)
            printf("too many args");
        *cmd = parseredir(*cmd, ps, es);
    }
    exec->argv[argc] = 0;
    exec->eargv[argc] = 0;
}

// t_cmd *parseexec(char **ps, char *es)
// {
//     t_cmd *cmd;

//     cmd = initexec(ps, es);
//     processargs((t_execcmd *)cmd, &cmd, ps, es);
//     return (cmd);
// }

t_cmd	*parseexec(char **ps, char *es)
{
	t_execcmd	*exec;
	t_cmd		*cmd;
    char *q, *eq;
    int tok, argc;

    argc = 0;
	cmd = execcmd();
	exec = (t_execcmd *)cmd;
	cmd = parseredir(cmd, ps, es);
    while (!peek(ps, es, "|"))
    {
        if ((tok = gettoken(ps, es, &q, &eq)) == 0)
            break;
        if (tok != 'a')
            printf("syntax");
        exec->argv[argc] = q;
        exec->eargv[argc] = eq;
        argc++;

        if (argc >= MAXARGS)
            printf("too many args");
        cmd = parseredir(cmd, ps, es);
    }
    exec->argv[argc] = 0;
    exec->eargv[argc] = 0;
	return (cmd);
}
