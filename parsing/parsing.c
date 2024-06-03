/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 12:27:36 by vlomakin          #+#    #+#             */
/*   Updated: 2024/06/03 13:53:37 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*execcmd(void)
{
	t_execcmd	*exec;

	exec = malloc(sizeof(*exec));
	ft_memset(exec, 0, sizeof(*exec));
	exec->type = EXEC;
	return ((t_cmd *)exec);
}

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
    while(!peek(ps, es, "|"))
    {
        if((tok=gettoken(ps, es, &q, &eq)) == 0)
            break;
        if(tok != 'a')
            printf("syntax");
        exec->argv[argc] = q;
        exec->eargv[argc] = eq;
        argc++;
        if(argc >= MAXARGS)
            printf("too many args");
        cmd = parseredir(cmd, ps, es);
    }
    exec->argv[argc] = 0;
    exec->eargv[argc] = 0;
	return (cmd);
}

t_cmd	*parsepipe(char **ps, char *es)
{
	t_cmd	*cmd;
    char *q, *eq;

	cmd = parseexec(ps, es);
    printf("\nParsed exec\n");
	if (peek(ps, es, "|"))
	{
        printf("Parse pipe\n");
		gettoken(ps, es, &q, &eq);
		cmd = pipecmd(cmd, parsepipe(ps, es));
	}
	return (cmd);
}

t_cmd	*parse(t_args *args)
{
	char	*es;
	char	*ps;
	t_cmd	*cmd;

	ps = args->input;
	es = ps + ft_strlen(args->input);
	cmd = parsepipe(&ps, es);
	printf("--------- parsepipe -----------\n");
	PrintTree(cmd);
	printf("--------------------------------\n");

	if (peek(&ps, es, ""))
    {
        if (ps != es)
            exit_with_err("Syntax err\n");
    }
	while(args->input < es && is_delimiter(*args->input))
		args->input++;
	nulterminate(cmd);

	printf("--------- nulterminate -----------\n");
	PrintTree(cmd);
	printf("----------------------------------\n");

	return (cmd);
}
