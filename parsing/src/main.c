/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aajaanan <aajaanan@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 14:53:51 by aajaanan          #+#    #+#             */
/*   Updated: 2023/09/06 07:34:07 by aajaanan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	execute_command(t_params *params, int *exit_status)
{
	int			status;
	t_execcmd	*ecmd;

	set_signal_handler(params->tree);
	if (forking(params) == 0)
	{
		save_child_pid(getpid(), params);
		run_cmd(params->tree, params, exit_status);
	}
	waitpid(-1, &status, 0);
	get_exit_status(params->tree, params, exit_status, status);
	ecmd = (t_execcmd *)params->tree;
	if (ecmd->type == EXEC && ft_strcmp(ecmd->argv[0], "exit") == 0)
	{
		if (!ecmd->argv[1] || (ecmd->argv[1]
				&& (!is_numeric(ecmd->argv[1]) || !ecmd->argv[2])))
		{
			unlink("/tmp/exit_status.tmp");
			unlink("/tmp/child_pid.tmp");
			free_exit(params, *exit_status);
		}
	}
	cleanup(params);
}

static void	initialize_shell_environment(t_params *params, int *exit_status,
		char **envp)
{
	*exit_status = 0;
	params->envp = envp;
	params->env_var_list = NULL;
	init_queue(&params->args_queue);
	init_env_var_list(envp, &params->env_var_list);
}

int	is_valid_shell_input(char *buf, int *exit_status)
{
	if (ft_strlen(buf) == 0 || is_whitespace_string(buf)
		|| !validate_command(buf, exit_status))
		return (0);
	return (1);
}

static void	execute_shell_command(t_params *params, int *exit_status)
{
	add_history(params->buf);
	params->tree = parse_cmd(params->buf, exit_status);
	if (!params->tree)
	{
		free1(params->buf);
		return ;
	}
	process_args(params->tree, params, exit_status);
	if (is_built_in_command(params->tree))
	{
		execute_built_in_command((t_execcmd *)params->tree,
			&params->env_var_list, exit_status);
		free1(params->buf);
		free_tree(params->tree);
		free_queue(&params->args_queue);
	}
	else
		execute_command(params, exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_params	params;
	int			exit_status;

	(void)argc;
	(void)argv;
	initialize_shell_environment(&params, &exit_status, envp);
	while (1)
	{
		setup_signals();
		params.buf = readline(BCYN "minishell$ " RESET);
		if (!params.buf)
		{
			ft_printf_fd(STDOUT_FILENO, "exit\n");
			break ;
		}
		if (!is_valid_shell_input(params.buf, &exit_status))
		{
			free1(params.buf);
			continue ;
		}
		execute_shell_command(&params, &exit_status);
	}
	free_env_var_list(params.env_var_list);
	rl_clear_history();
	return (exit_status);
}
