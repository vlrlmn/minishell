/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aajaanan <aajaanan@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 08:42:53 by aajaanan          #+#    #+#             */
/*   Updated: 2023/09/02 08:52:11 by aajaanan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_built_in_command(t_cmd *tree)
{
	t_execcmd	*ecmd;

	if (tree && tree->type == EXEC)
	{
		ecmd = (t_execcmd *)tree;
		if (ft_strcmp(ecmd->argv[0], "export") == 0)
			return (1);
		else if (ft_strcmp(ecmd->argv[0], "unset") == 0)
			return (1);
		else if (ft_strcmp(ecmd->argv[0], "cd") == 0)
			return (1);
	}
	return (0);
}

void	execute_built_in_command(t_execcmd *ecmd, t_env_var **env_var_list,
		int *exit_status)
{
	if (ft_strcmp(ecmd->argv[0], "export") == 0)
		export_command(ecmd->argv, env_var_list, exit_status);
	else if (ft_strcmp(ecmd->argv[0], "unset") == 0)
		unset_env_var(ecmd->argv, env_var_list, exit_status);
	else if (ft_strcmp(ecmd->argv[0], "cd") == 0)
		cd(ecmd->argv, exit_status, *env_var_list);
}

void	save_child_pid(int pid, t_params *params)
{
	int	fd;

	fd = open("/tmp/child_pid.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		free_panic_exit(params, "open", 1);
	if (write(fd, &pid, sizeof(int)) < 0)
		free_panic_exit(params, "write", 1);
	close(fd);
}

void	get_exit_status(t_cmd *tree, t_params *params, int *exit_status,
		int status)
{
	int	fd;

	if (tree && (tree->type == EXEC || tree->type == REDIR))
	{
		if (WIFEXITED(status))
			*exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*exit_status = WTERMSIG(status) + 128;
		else
			*exit_status = 1;
	}
	else
	{
		fd = open("/tmp/exit_status.tmp", O_RDONLY);
		if (fd < 0)
			free_panic_exit(params, "open", 1);
		if (read(fd, exit_status, sizeof(int)) < 0)
			free_panic_exit(params, "read", 1);
		close(fd);
	}
}

void	cleanup(t_params *params)
{
	unlink("/tmp/exit_status.tmp");
	unlink("/tmp/child_pid.tmp");
	free1(params->buf);
	free_tree(params->tree);
	free_queue(&params->args_queue);
}
