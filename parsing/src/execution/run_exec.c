/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aajaanan <aajaanan@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/26 15:30:45 by aajaanan          #+#    #+#             */
/*   Updated: 2023/09/04 09:39:55 by aajaanan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

static int	is_builtin_command(t_execcmd *ecmd)
{
	if (ft_strcmp(ecmd->argv[0], "echo") == 0)
		return (1);
	else if (ft_strcmp(ecmd->argv[0], "exit") == 0)
		return (1);
	else if (ft_strcmp(ecmd->argv[0], "env") == 0)
		return (1);
	else if (ft_strcmp(ecmd->argv[0], "export") == 0)
		return (1);
	else if (ft_strcmp(ecmd->argv[0], "unset") == 0)
		return (1);
	else if (ft_strcmp(ecmd->argv[0], "pwd") == 0)
		return (1);
	else
		return (0);
}

static void	execute_builtin_commands(t_execcmd *ecmd, t_params *params,
	int exit_status)
{
	if (ft_strcmp(ecmd->argv[0], "echo") == 0)
		echo(ecmd->argv);
	else if (ft_strcmp(ecmd->argv[0], "exit") == 0)
		exit_command(ecmd->argv, params);
	else if (ft_strcmp(ecmd->argv[0], "env") == 0)
		env(ecmd->argv, params);
	else if (ft_strcmp(ecmd->argv[0], "export") == 0)
		export(ecmd->argv, params->env_var_list);
	else if (ft_strcmp(ecmd->argv[0], "unset") == 0)
		free_exit(params, 0);
	else if (ft_strcmp(ecmd->argv[0], "pwd") == 0)
		pwd(&exit_status);
}

static void	execute_external_command(t_cmd *cmd, t_params *params)
{
	t_execcmd	*ecmd;
	char		*path_var;
	char		*binary_path;

	binary_path = NULL;
	ecmd = (t_execcmd *)cmd;
	path_var = getenv_value("PATH", params->env_var_list);
	if (path_var == NULL)
	{
		ft_printf_fd(STDERR_FILENO, "minishell: %s: No such file or directory\n",
			ecmd->argv[0]);
		free_exit(params, 127);
	}
	binary_path = find_command_path(ecmd->argv[0], path_var);
	if (binary_path == NULL)
	{
		ft_printf_fd(STDERR_FILENO, "minishell: %s: command not found\n",
			ecmd->argv[0]);
		free_exit(params, 127);
	}
	execve(binary_path, ecmd->argv, params->envp);
	free1(binary_path);
	free_exit(params, 126);
}

void	run_exec(t_cmd *cmd, t_params *params, int *exit_status)
{
	t_execcmd	*ecmd;

	ecmd = (t_execcmd *)cmd;
	remove_empty_args(ecmd);
	handle_executable_path(ecmd, params);
	if (is_builtin_command(ecmd))
	{
		execute_builtin_commands(ecmd, params, *exit_status);
		free_exit(params, 0);
	}
	else
	{
		if (access(ecmd->argv[0], X_OK) == 0)
		{
			execve(ecmd->argv[0], ecmd->argv, params->envp);
			ft_printf_fd(STDERR_FILENO, "minishell: %s: command not found\n",
				ecmd->argv[0]);
			free_exit(params, 127);
		}
		else
			execute_external_command(cmd, params);
	}
}
