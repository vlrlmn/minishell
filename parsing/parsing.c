/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 12:27:36 by vlomakin          #+#    #+#             */
/*   Updated: 2024/05/28 11:27:19 by lomakinaval      ###   ########.fr       */
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

void	parse_args(t_cmd **pcmd, t_execcmd *exec, char **ps, char *es)
{
	char	*q;
	char	*eq;
	int		argc;
	int		tok;

	argc = 0;
	while (!peek(ps, es, "|"))
	{
		tok = gettoken(ps, es, &q, &eq);
		if (tok == 0)
			break ;
		if (tok != 'a')
			exit(SYNTAX_ERR);
		printf("Token %d: %.*s\n", argc, (int)(eq - q), q);
		exec->argv[argc] = q;
		exec->eargv[argc] = eq;
		// Debugging message to check what is being stored in argv
        printf("Argument %d: %.*s\n", argc, (int)(eq - q), q);
		argc++;
		if (argc >= MAXARGS)
		{
			exit_with_err("Too many args\n"); //HANDLE ERRORS - MAYBE RETURN ERROR STATUS NUM
			exit (SYNTAX_ERR);
		}
		*pcmd = parseredir(*pcmd, ps, es);
	}
	exec->argv[argc] = 0;
	exec->eargv[argc] = 0;
	printf("Total args: %d\n", argc);
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
	t_cmd	*cmd;

	es = args->input + ft_strlen(args->input);
	cmd = parsepipe(&args->input, es);
	while(args->input < es && is_delimiter(*args->input))
		args->input++;
	if (args->input != es)
		exit_with_syntax_err(args, SYNTAX_ERR);
	nulterminate(cmd);
	return (cmd);
}
