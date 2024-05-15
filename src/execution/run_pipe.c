/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aajaanan <aajaanan@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 13:13:57 by aajaanan          #+#    #+#             */
/*   Updated: 2023/09/06 19:01:24 by aajaanan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"
#include <sys/wait.h>

static void	execute_left_subtree(t_cmd *cmd, int fd[2], t_params *params,
	int *exit_status)
{
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	run_cmd(cmd, params, exit_status);
	exit(0);
}

static void	execute_right_subtree(t_cmd *cmd, int fd[2], t_params *params,
	int *exit_status)
{
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	run_cmd(cmd, params, exit_status);
	exit(0);
}

static void	write_exit_status_to_file(t_params *params, int exit_status)
{
	int	fd;

	fd = open("/tmp/exit_status.tmp", O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (fd < 0)
		free_panic_exit(params, "open", 1);
	if (write(fd, &exit_status, sizeof(int)) < 0)
		free_panic_exit(params, "write", 1);
	close(fd);
}

static void	set_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	run_pipe(t_cmd *cmd, t_params *params, int *exit_status)
{
	int			fd[2];
	int			pid1;
	int			pid2;
	int			status;
	int			is_herdoc = 0;
	t_pipecmd	*pcmd;

	pcmd = (t_pipecmd *)cmd;
	pipe1(fd, params);
	pid1 = forking(params);
	if (pid1 == 0)
		execute_left_subtree(pcmd->left, fd, params, exit_status);
	if (pcmd->left->type == REDIR && ((t_redircmd *)pcmd->left)->r_type == '%')
	{
		close(fd[0]);
		close(fd[1]);
		waitpid(pid1, NULL, 0);
		is_herdoc = 1;
	}
	pid2 = forking(params);
	if (pid2 == 0)
		execute_right_subtree(pcmd->right, fd, params, exit_status);
	set_signals();
	if (!is_herdoc)
	{
		close(fd[0]);
		close(fd[1]);
		waitpid(pid1, NULL, 0);
	}
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		*exit_status = WEXITSTATUS(status);
	else
		*exit_status = 1;
	write_exit_status_to_file(params, *exit_status);
	free_exit(params, *exit_status);
}
