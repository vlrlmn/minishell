/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_buildin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:53:53 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/03 14:36:41 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int run_buildin(t_cmd_info *ecmd, t_args *params, t_cmd_info *cmd_list, int **pipe_arr)
{
    int status;

    status = -1;
    // int i = 0;
    // while (ecmd->argv[i])
    // {
    //     printf("%d arg: %s\n", i, ecmd->argv[i]);
    //     i++;
    // }
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
        // cmd is not valis
        return 1;
    }
    return (status);
}

int is_buildin(char *cmd)
{
 	return (ft_strncmp(cmd, "cd", 2) == 0 || ft_strncmp(cmd, "exit", 2) == 0 || ft_strncmp(cmd, "echo", 2) == 0
				|| ft_strncmp(cmd, "pwd", 2) == 0 || ft_strncmp(cmd, "export", 2) == 0 || ft_strncmp(cmd, "env", 2) == 0
					|| ft_strncmp(cmd, "unset", 2) == 0);
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
    
	// fprintf(stderr, "Running command: %s\n", cmd->argv[0]); // Debug message
    // do dup2 for output redirection
	builtin_status = run_buildin(cmd, params, cmd_list, pipe_arr);
	return (builtin_status);
}