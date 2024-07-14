/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 12:27:36 by vlomakin          #+#    #+#             */
/*   Updated: 2024/07/14 12:34:19 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*parsepipe(char **ps, char *es)
{
	t_cmd	*cmd;
	char *q, *eq;

	cmd = parseexec(ps, es);
	if (peek(ps, es, "|"))
	{
		gettoken(ps, es, &q, &eq);
		cmd = pipecmd(cmd, parsepipe(ps, es));
	}
	return (cmd);
}

t_cmd	*parse(t_args *args, int *exit_status)
{
	char	*es;
	char	*ps;
	t_cmd	*cmd;

	ps = args->input;
	es = ps + ft_strlen(args->input);
	cmd = parsepipe(&ps, es);
	if (peek(&ps, es, ""))
    {
        if (ps != es)
            exit_with_err("Syntax err\n");
    }
	while (args->input < es && is_delimiter(*args->input))
		args->input++;
	nulterminate(cmd);
	lexical_analysis(cmd, args, exit_status);
	return (cmd);
}
