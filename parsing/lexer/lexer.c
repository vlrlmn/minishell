/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 17:36:36 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/12 13:02:57 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	parse_empty_quotes(int i, char *line, t_lexems *list)
{
	if (i == 2 && line[0] == '\'' && line[i - 1] == '\'')
	{
		add_str_node(list, "\'\'");
	}
	if (i == 2 && line[0] == '\"' && line[i - 1] == '\"')
	{
		add_str_node(list, " ");
	}
}

char	*clean_line(char *line, t_lexems *list, t_args *args)
{
	char	*res;
	int		i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\'')
			parse_quote(line, &i, list);
		else if (line[i] == '\"')
			parse_double_quote(&i, line, list, args);
		else if (line[i] == '$')
			parse_expander(&i, list, line, args);
		else
		{
			add_char_node(list, line[i]);
			i++;
		}
	}
	parse_empty_quotes(i, line, list);
	res = list_to_string(list);
	return (res);
}

char	*clean_cmd(char *line, t_args *args)
{
	t_lexems	args_list;
	char		*val;

	args_list.head = NULL;
	args_list.tail = NULL;
	if (ft_strchr(line, '~') && !ft_isalnum((*line + 1)))
	{
		val = ft_strdup(get_env("HOME", args->envp));
		return (val);
	}
	val = clean_line(line, &args_list, args);
	return (val);
}

void	lexer_exec(t_cmd *cmd, t_args *args)
{
	t_execcmd	*exec;
	int			i;

	i = 0;
	exec = (t_execcmd *)cmd;
	while (exec->argv[i])
	{
		if (has_parse_symbol(exec->argv[i]))
			exec->argv[i] = clean_cmd(exec->argv[i], args);
		else
			exec->argv[i] = ft_strdup(exec->argv[i]);
		i++;
	}
}

void	lexical_analysis(t_cmd *cmd, t_args *args)
{
	t_redir	*rcmd;
	t_pipe	*pcmd;

	if (cmd->type == EXEC)
		lexer_exec(cmd, args);
	else if (cmd->type == REDIR)
	{
		rcmd = (t_redir *)cmd;
		if (rcmd->type == '-' && has_parse_symbol(rcmd->file))
			rcmd->file = clean_cmd(rcmd->file, args);
		lexical_analysis(rcmd->cmd, args);
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipe *)cmd;
		lexical_analysis(pcmd->left, args);
		lexical_analysis(pcmd->right, args);
	}
}
