/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 12:27:36 by vlomakin          #+#    #+#             */
/*   Updated: 2024/05/30 12:58:25 by lomakinaval      ###   ########.fr       */
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
void parse_args(t_cmd **pcmd, t_execcmd *exec, char **ps, char *es)
{
    char *q;
    char *eq;
    int argc;
    int tok;

    argc = 0;
    while (!peek(ps, es, "|"))
    {
        tok = gettoken(ps, es, &q, &eq);
        if (tok == 0)
            break;
        // if (tok != 'a')
        //     exit(EXIT_FAILURE);
        printf("\nToken %d: %.*s\n", argc, (int)(eq - q), q); // Debug message
        exec->argv[argc] = q;
        exec->eargv[argc] = eq;
        printf("\nArgument %d: %.*s\n", argc, (int)(eq - q), q); // Debug message
        argc++;
        if (argc >= MAXARGS)
        {
            exit_with_err("Too many args\n"); //HANDLE ERRORS - MAYBE RETURN ERROR STATUS NUM
            exit(EXIT_FAILURE);
        }
        *pcmd = parseredir(*pcmd, ps, es);
    }
    exec->argv[argc] = 0;
    exec->eargv[argc] = 0;
    printf("\nTotal args: %d\n", argc); //DEBUGGING MESSAGE
    printf("\nargv = {"); //DEBUGGING MESSAGE
    for (int i = 0; i < argc; i++) //DEBUGGING MESSAGE
    {
        printf("\"%.*s\"", (int)(exec->eargv[i] - exec->argv[i]), exec->argv[i]);
        if (i < argc - 1)
            printf(", ");
    }
    printf(", NULL}\n"); //DEBUGGING MESSAGE
    printf("\neargv = {");//DEBUGGING MESSAGE
    for (int i = 0; i < argc; i++) //DEBUGGING MESSAGE
    {
        printf("end \"%.*s\"", (int)(exec->eargv[i] - exec->argv[i]), exec->argv[i]);
        if (i < argc - 1)
            printf(", ");
    }
    printf(", NULL}\n\n");//DEBUGGING MESSAGE
}

t_cmd	*parseexec(char **ps, char *es)
{
	t_execcmd	*exec;
	t_cmd		*cmd;

	cmd = execcmd();
	exec = (t_execcmd *)cmd;
	cmd = parseredir(cmd, ps, es);
	parse_args(&cmd, exec, ps, es);
	return (cmd);
}

t_cmd	*parsepipe(char **ps, char *es)
{
	t_cmd	*cmd;

	cmd = parseexec(ps, es);
	if (peek(ps, es, "|"))
	{
		gettoken(ps, es, 0, 0);
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
	peek(&ps, es, "");
	if (ps != es)
		exit_with_err("Syntax err\n");
	while(args->input < es && is_delimiter(*args->input))
		args->input++;
	nulterminate(cmd);
	return (cmd);
}
