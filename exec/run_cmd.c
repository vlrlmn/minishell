/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:26:37 by vlomakin          #+#    #+#             */
/*   Updated: 2024/06/18 17:17:14 by sabdulki         ###   ########.fr       */
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
	pid1 = fork();
	if (pid1 == -1)
		exit_with_err("fork");
	if (pid1 == 0)
	{
		printf("Fork for left\n");
		close(p[0]);
		printf("Fork for left1 \n");
		// PrintTree(pcmd->left);
		dup2(p[1], STDOUT_FILENO);
		//PrintTree(pcmd->left);
		//printf("Fork for left2 \n");
		close(p[1]);
		fprintf(stderr, "Run Left\n");
		run_cmd(pcmd->left, params);
		fprintf(stderr, "FINISH RUN LEFT\n");
		exit(0);
	}
	
	pid2 = fork();
	if (pid1 == -1)
		exit_with_err("fork");
	if (pid2 == 0)
	{
		printf("Fork for right\n");
        close(p[1]);
	    dup2(p[0], STDIN_FILENO);
	    close(p[0]);
		fprintf(stderr, "Run right\n");
		run_cmd(pcmd->right, params);
		exit(0);
	}
	close(p[0]);
	close(p[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	printf("\npid2 STATUS %d\n", status);
}

/* TODO : the file descriptor is leaking!!! it showes PrintTree() result in the file */
void	run_redir(t_cmd *cmd, t_args *params)
{
	t_redir *rcmd;

	rcmd = (t_redir *)cmd;
	fprintf(stderr, "\tfd in run_redir before redir(): %d\n", rcmd->fd);
	close(rcmd->fd); //This ensures that the file descriptor is available to be reused.
	fprintf(stderr, "\nRCMD FILE in exec: '%s'\n", rcmd->file);
	fprintf(stderr, "fd: %d\n", rcmd->fd);
	PrintTree(rcmd->cmd);
	redir(rcmd);
	fprintf(stderr, "\tfd after redir(): %d\n", rcmd->fd);
	run_cmd(rcmd->cmd, params); //it calls run_cmd to execute the sub-command (rcmd->cmd)
}

void check_arguments(t_execcmd *ecmd)
{
	int i;
	int j;

	i = 0;
	j = 0;
    while (ecmd->argv[i])
    {
        if (ecmd->argv[i][0] != '\0')
        {
            if (i != j)
            {
                ecmd->argv[j] = ecmd->argv[i];
                ecmd->eargv[j] = ecmd->eargv[i];
            }
            j++;
        }
        i++;
    }
    ecmd->argv[j] = NULL;
    ecmd->eargv[j] = NULL;
}

// void	run_exec_cmd(t_execcmd	*ecmd, t_args *params)
// {
// 	int		i;
// 	int		status;
// 	char	*cmd_path;
// 	char	*path;

// 	i = 0;
// 	cmd_path = NULL;
// 	while (ecmd->argv[i])
// 	{
// 		fprintf(stderr, "Arg %d: %.*s\n", i, (int)(ecmd->eargv[i] - ecmd->argv[i]), ecmd->argv[i]);
// 		i++;
// 	}
// 	fprintf(stderr, "Executing command: %s\n", ecmd->argv[0]); // Debug message
// 	if (!params)
// 	{
// 		fprintf(stderr, "!cmd->params && !cmd->params->envp\n");
// 	}
// 	path = get_env("PATH=", params->envp);
// 	cmd_path = find_command_path(ecmd->argv[0], path);
// 	if (!cmd_path)
// 	{
// 		fprintf(stderr, "Command not found: %s\n", ecmd->argv[0]);
// 		exit(127);
// 	}
// 	//current program ceases to exist and the new program takes its place
// 	status = execve(cmd_path, ecmd->argv, params->envp);
// 	fprintf(stderr,"\texecve!");
// 	free(cmd_path);
// 	exit(status);
// 	//exit only if run_exec_cmd failed (??)
// 	//had to add one more exit from the child proc if it was successfull!!
// }

void	close_fd(t_cmd *ecmd)
{
	t_redir *rcmd;

	rcmd = (t_redir *)ecmd;
	if (rcmd->subtype == HEREDOC)
	{
		close(rcmd->fd);
		fprintf(stderr, "closed fd!\n");
	}
}

void check_close(int fd) {
    if (close(fd) == -1) {
        fprintf(stderr, "Error closing file descriptor %d: %s\n", fd, strerror(errno));
    } else {
        fprintf(stderr, "Successfully closed file descriptor %d\n", fd);
    }
}

void check_dup2(int oldfd, int newfd) {
    if (dup2(oldfd, newfd) == -1) {
        fprintf(stderr, "Error duplicating file descriptor from %d to %d: %s\n", oldfd, newfd, strerror(errno));
    } else {
        fprintf(stderr, "Successfully duplicated file descriptor from %d to %d\n", oldfd, newfd);
    }
}

void	run_exec(t_cmd *cmd, t_args *params)
{
	t_execcmd	*ecmd;
	// t_redir *rcmd;
	
	int	builtin_status;
	char	*cmd_path;
	char	*path;
	// pid_t pid;

	cmd_path = NULL;

	// rcmd = (t_redir *)cmd;
	// PrintTree(cmd);
	// fprintf(stderr, "rcmd->subtype in exec: %d\n", rcmd->subtype);
	// if (rcmd->subtype == HEREDOC)
	// {
	// 	check_dup2(rcmd->fd, STDIN_FILENO);
	// 	check_close(rcmd->fd);
	// 	fprintf(stderr, "\tchanged the fd!\n");
	// }
	// cmd = (t_cmd *)rcmd;

	ecmd = (t_execcmd *)cmd;
	if (ecmd->argv[0] == 0)
		exit(127);
	check_arguments(ecmd);
	if (is_buildin(ecmd->argv[0]))
	{
		builtin_status = run_single_builtin(cmd, params);
		if (builtin_status == 0)
			return ;
		else if (builtin_status == 1)
			exit_with_err("Command not executed\n");
		fprintf(stderr, "STATUS %d\n", builtin_status);
	}
	else
	{
		fprintf(stderr, "In run_exec function!\n");
		int i = 0;
		while (ecmd->argv[i])
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
		if (!cmd_path)
		{
			fprintf(stderr, "Command not found: %s\n", ecmd->argv[0]);
			exit(127);
		}
		fprintf(stderr, "Found the path! : %s\n", cmd_path);
		execve(cmd_path, ecmd->argv, params->envp);
		fprintf(stderr, "execve errno:%d !\n", errno);
	}
	close_fd((t_cmd *)ecmd); //the function is not executing there idk why
	free(cmd_path);
	exit(126);
}



void run_cmd(t_cmd *cmd, t_args *params)
{
    if (!cmd) {
        printf("run_cmd: cmd is NULL\n");
        exit(127);
    }
    fprintf(stderr, "Running command type: %d\n", cmd->type);
	// create linked list there and fill it recursively
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
	return ;
}
