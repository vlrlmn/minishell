/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 17:36:36 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/14 12:28:54 by lomakinaval      ###   ########.fr       */
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

char	*clean_line(t_clean_line_args clean_args, int *exit_status)
{
	char	*res;
	int		i;

	i = 0;
	while (clean_args.line[i])
	{
		if (clean_args.line[i] == '\'')
			parse_quote(clean_args.line, &i, clean_args.list);
		else if (clean_args.line[i] == '\"')
			parse_double_quote(&i, clean_args, exit_status);
		else if (clean_args.line[i] == '$')
			parse_expander(&i, clean_args, exit_status);
		else
		{
			add_char_node(clean_args.list, clean_args.line[i]);
			i++;
		}
	}
	parse_empty_quotes(i, clean_args.line, clean_args.list);
	res = list_to_string(clean_args.list);
	return (res);
}

char	*clean_cmd(char *line, t_args *args, int *exit_status)
{
	t_lexems	args_list;
	char		*val;
	t_clean_line_args clean_args;

	args_list.head = NULL;
	args_list.tail = NULL;
	if (ft_strchr(line, '~') && !ft_isalnum((*line + 1)))
	{
		val = ft_strdup(get_env("HOME", args->envp));
		return (val);
	}
	clean_args.line = line;
	clean_args.list = &args_list;
	clean_args.args = args;
	val = clean_line(clean_args, exit_status);
	return (val);
}

void	lexer_exec(t_cmd *cmd, t_args *args, int *exit_status)
{
	t_execcmd	*exec;
	int			i;

	i = 0;
	exec = (t_execcmd *)cmd;
	while (exec->argv[i])
	{
		if (has_parse_symbol(exec->argv[i]))
			exec->argv[i] = clean_cmd(exec->argv[i], args, exit_status);
		else
			exec->argv[i] = ft_strdup(exec->argv[i]);
		i++;
	}
}

void	lexical_analysis(t_cmd *cmd, t_args *args, int *exit_status)
{
	t_redir	*rcmd;
	t_pipe	*pcmd;

	if (cmd->type == EXEC)
		lexer_exec(cmd, args, exit_status);
	else if (cmd->type == REDIR)
	{
		rcmd = (t_redir *)cmd;
		if (rcmd->type == '-' && has_parse_symbol(rcmd->file))
			rcmd->file = clean_cmd(rcmd->file, args, exit_status);
		lexical_analysis(rcmd->cmd, args, exit_status);
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipe *)cmd;
		lexical_analysis(pcmd->left, args, exit_status);
		lexical_analysis(pcmd->right, args, exit_status);
	}
}
