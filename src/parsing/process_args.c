/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aajaanan <aajaanan@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 08:07:26 by aajaanan          #+#    #+#             */
/*   Updated: 2023/09/02 12:05:11 by aajaanan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parsing.h"

static void	get_clean_arg_2(char *arg, t_queue_char *q, int *exit_status,
		t_params *params)
{
	int	i;
	int	*values[2];

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '\'')
			process_single_quote(arg, &i, q);
		else if (arg[i] == '\"')
		{
			values[0] = &i;
			values[1] = exit_status;
			process_double_quote(values, arg, q, params);
		}
		else if (arg[i] == '$')
		{
			values[0] = &i;
			values[1] = exit_status;
			process_dollar_sign(values, q, arg, params);
		}
		else
			enqueue_char(q, arg[i++]);
	}
}

static char	*get_clean_arg(char *arg, t_params *params, int *exit_status)
{
	t_queue_char	q;
	char			*clean_arg;

	init_queue_char(&q);
	if (ft_strcmp(arg, "~") == 0)
	{
		clean_arg = ft_strdup(getenv_value("HOME", params->env_var_list));
		enqueue(&params->args_queue, clean_arg);
		return (clean_arg);
	}
	get_clean_arg_2(arg, &q, exit_status, params);
	clean_arg = queue_char_to_str(&q);
	enqueue(&params->args_queue, clean_arg);
	return (clean_arg);
}

static void	process_args_exec(t_cmd *cmd, t_params *params, int *exit_status)
{
	int			i;
	t_execcmd	*ecmd;

	i = 0;
	ecmd = (t_execcmd *)cmd;
	while (ecmd->argv[i])
	{
		if (contains_special_char(ecmd->argv[i]))
			ecmd->argv[i] = get_clean_arg(ecmd->argv[i], params, exit_status);
		i++;
	}
}

void	process_args(t_cmd *cmd, t_params *params, int *exit_status)
{
	t_pipecmd	*pcmd;
	t_redircmd	*rcmd;

	if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd *)cmd;
		process_args(pcmd->left, params, exit_status);
		process_args(pcmd->right, params, exit_status);
	}
	else if (cmd->type == REDIR)
	{
		rcmd = (t_redircmd *)cmd;
		if (rcmd->r_type != '%' && contains_special_char(rcmd->file))
			rcmd->file = get_clean_arg(rcmd->file, params, exit_status);
		process_args(rcmd->subcmd, params, exit_status);
	}
	else if (cmd->type == EXEC)
		process_args_exec(cmd, params, exit_status);
}
