/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_buildin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:53:53 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/05 13:02:27 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int run_buildin(t_cmd_info *ecmd, t_args *params, t_cmd_info *cmd_list, int **pipe_arr)
{
    int status;

    status = -1;
    int i = 0;
    while (ecmd->argv[i])
    {
        // printf("%d arg: %s\n", i, ecmd->argv[i]);
        i++;
    }
    if (ft_strncmp(ecmd->argv[0], "cd", 2) == 0)
        status = cd_cmd(ecmd, params);
    else if (ft_strncmp(ecmd->argv[0], "exit", 4) == 0)
        exit_cmd(ecmd, params, cmd_list, pipe_arr);
    else if (ft_strncmp(ecmd->argv[0], "echo", 4) == 0)
        status = echo_cmd(ecmd);
    else if (ft_strncmp(ecmd->argv[0], "pwd", 3) == 0)
        status = pwd_cmd(ecmd, params);
    else if (ft_strncmp(ecmd->argv[0], "export", 6) == 0)
        status =  pre_export_cmd(ecmd, params);
    else if (ft_strncmp(ecmd->argv[0], "env", 3) == 0)
        status = env_cmd(ecmd, params);
    else if (ft_strncmp(ecmd->argv[0], "unset", 5) == 0)
        status = pre_unset_cmd(ecmd, params);
    else
    {
        // cmd is not valid
        fprintf(stderr, "Command not found: %s\n", ecmd->argv[0]);
        return (127);
    }
    return (status);
}

int is_buildin(char *cmd)
{
    int len;

    if (!cmd)
        return (0);
    len = ft_strlen(cmd);
 	return (ft_strncmp(cmd, "cd", len) == 0 || ft_strncmp(cmd, "exit", len) == 0 || ft_strncmp(cmd, "echo", len) == 0
				|| ft_strncmp(cmd, "pwd", len) == 0 || ft_strncmp(cmd, "export", len) == 0 || ft_strncmp(cmd, "env", len) == 0
					|| ft_strncmp(cmd, "unset", len) == 0);
}

int check_if_single_builtin(t_cmd_info *cmd)
{
	if (cmd->argv[0] == 0)
		exit(127);
	// or better if (ecmd->type != EXEC && ecmd->type != Redir && is_buildin(ecmd->argv[0]))
	return (cmd->type == EXEC && is_buildin(cmd->argv[0]));
}

int run_single_builtin(t_cmd_info *cmd, t_args *params, t_cmd_info *cmd_list, int **pipe_arr)
{
	int	builtin_status;
    // int saved_stdout;
    
	// fprintf(stderr, "Running command: %s\n", cmd->argv[0]); // Debug message
    // do dup2 for output redirection
    // saved_stdout = dup(STDOUT_FILENO);
    // if (saved_stdout < 0) {
    //     perror("dup");
    //     // close fd-s, free all, return 1
    //     return (1);
    // }
    // write(1, "after dup()\n", 13);
    // if (dup2(cmd->fd_write, STDOUT_FILENO) < 0) {
    //     perror("dup2");
    //     close(saved_stdout);
    //     // close fd-s, free all, return 1
    //     return (1);
    // }
    // write(1, "after 1 dup2()\n", 16);
	builtin_status = run_buildin(cmd, params, cmd_list, pipe_arr);
    // if (dup2(saved_stdout, STDOUT_FILENO) < 0) {
    //     perror("dup2");
    //     close(saved_stdout);
    //     // close fd-s, free all, return 1
    //     return (1);
    // }
    // write(1, "after 2 dup2()\n", 16);
    // close(saved_stdout);
	return (builtin_status);
}