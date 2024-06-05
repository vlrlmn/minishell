/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:26:37 by vlomakin          #+#    #+#             */
/*   Updated: 2024/06/05 16:38:21 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	run_pipe(t_cmd *cmd, t_args *params)
{
	t_pipe	*pcmd;
	int	p[2];
	int pid1;
	int pid2;
	int status;

	pcmd = (t_pipe *)cmd;
	if (pipe(p) < 0)
		exit_with_err("Pipe error");
	pid1 = fork1();
	if (pid1 == 0)
	{
		printf("Fork for left\n");
		close(p[0]);
		printf("Fork for left1 \n");
		PrintTree(pcmd->left);
		dup2(p[1], STDOUT_FILENO);
		//PrintTree(pcmd->left);
		//printf("Fork for left2 \n");
		close(p[1]);
		fprintf(stderr, "Run Left\n");
		run_cmd(pcmd->left, params);
		fprintf(stderr, "FINISH RUN LEFT\n");
		exit(0);
	}
	
	pid2 = fork1();
	if (pid2 == 0)
	{
		printf("Fork for right\n");
        close(p[1]);
	    dup2(p[0], STDIN_FILENO);
	    close(p[0]);
		printf("Run right\n");
		run_cmd(pcmd->right, params);
		exit(0);
	}
	close(p[0]);
	close(p[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	printf("\npid2 STATUS %d\n", status);
}


void	run_redir(t_cmd *cmd, t_args *params)
{
	t_redir *rcmd;

	rcmd = (t_redir *)cmd;
	close(rcmd->fd);
	fprintf(stderr, "\nRCMD FILE %s\n", rcmd->file);
	PrintTree(rcmd->cmd);
	if (rcmd->type == '-')
	{
		exec_heredoc(); // TO_DO HEREDOC
	}
	else
	{
		if(open(rcmd->file, rcmd->mode, 0644) < 0)
		{
			printf("open %s failed\n", rcmd->file);
			exit(126);
		}
		run_cmd(rcmd->cmd, params);
	}
}

int is_buildin(char *cmd)
{
 	return (ft_strncmp(cmd, "cd", 2) == 0 || ft_strncmp(cmd, "exit", 2) == 0 || ft_strncmp(cmd, "echo", 2) == 0
				|| ft_strncmp(cmd, "pwd", 2) == 0 || ft_strncmp(cmd, "export", 2) == 0 || ft_strncmp(cmd, "env", 2) == 0
					|| ft_strncmp(cmd, "unset", 2) == 0);
}

char	*get_env(char *path, char **envp)
{
	int i;
    // fprintf(stderr, "get_env in \n");
	i = 0;
	while (envp[i])
	{
		// fprintf(stderr, "envp[%d]: %s \n",i, envp[i]);
		if (ft_strncmp(path, envp[i], 5) == 0)
			return (envp[i]);
		i++;
	}
	// fprintf(stderr, "get_env out \n");
	return (NULL);
}

void	run_exec(t_cmd *cmd, t_args *params)
{
	t_execcmd	*ecmd;
	char	*cmd_path;
	int	builtin_status;
	char *path;
	int i;

	ecmd = (t_execcmd *)cmd;
	cmd_path = NULL;
	if (ecmd->argv[0] == 0)
		exit(127);
	if (is_buildin(ecmd->argv[0]))
	{
		fprintf(stderr, "Running command: %s\n", ecmd->argv[0]); // Debug message
		builtin_status = run_buildin(ecmd, params);
		if (builtin_status == 0)
			exit (0);
		else if (builtin_status == 1)
			exit_with_err("Command not executed\n");
		fprintf(stderr, "STATUS %d\n", builtin_status);
	}
	else
	{

		i=0;
		while(ecmd->argv[i])
		{
			fprintf(stderr, "Arg %d: %.*s\n", i, (int)(ecmd->eargv[i] - ecmd->argv[i]), ecmd->argv[i]);
			i++;
		}
		fprintf(stderr, "Executing command: %s\n", ecmd->argv[0]); // Debug message
		if (!params)
		{
			fprintf(stderr, "!cmd->params \n");
		}
		if (!params)
		{
			fprintf(stderr, "!cmd->params->envp \n");
		}
		path = get_env("PATH=", params->envp);
		// fprintf(stderr, "get_env \n");
		cmd_path = find_command_path(ecmd->argv[0], path);
		// fprintf(stderr, "find_command_path \n");
		if(!cmd_path)
		{
			fprintf(stderr, "Command not found: %s\n", ecmd->argv[0]);
			exit(127);
		}
		execve(cmd_path, ecmd->argv, params->envp);
		perror("execve");
	}
	free(cmd_path);
	exit(126);
}

void run_cmd(t_cmd *cmd, t_args *params)
{
    if (!cmd) {
        printf("run_cmd: cmd is NULL\n");
        exit(127);
    }
    //printf("Running command type: %d\n", cmd->type);
    if (cmd->type == EXEC)
        run_exec(cmd, params);
    else if (cmd->type == REDIR)
        run_redir(cmd, params);
    else if (cmd->type == PIPE)
        run_pipe(cmd, params);
    else {
        printf("run_cmd: Unknown command type\n");
        exit(127);
    }
}

