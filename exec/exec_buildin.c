/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_buildin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:53:53 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/06/14 15:40:58 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int run_buildin(t_execcmd *ecmd, t_args *params)
{
    int status;

    status = -1;
    int i = 0;
    while(ecmd->argv[i])
    {
        printf("%d arg: %s\n", i, ecmd->argv[i]);
        i++;
    }
    if (ft_strncmp(ecmd->argv[0], "cd", 2) == 0)
        status = cd_cmd(ecmd, params); //DEBUG
    else if (ft_strncmp(ecmd->argv[0], "exit", 4) == 0)
    {
        free_envp(params);
        exit(0);
    }
    else if (ft_strncmp(ecmd->argv[0], "echo", 4) == 0)
        status = echo_cmd(ecmd);
    else if (ft_strncmp(ecmd->argv[0], "pwd", 3) == 0)
        status = pwd_cmd(ecmd, params);
    else if (ft_strncmp(ecmd->argv[0], "export", 6) == 0)
        status = export_cmd(ecmd, params);
    else if (ft_strncmp(ecmd->argv[0], "env", 3) == 0)
        status = env_cmd(ecmd, params);
    else if (ft_strncmp(ecmd->argv[0], "unset", 5) == 0)
        status = unset_cmd(ecmd, params);
    return(status);
}

int is_buildin(char *cmd)
{
 	return (ft_strncmp(cmd, "cd", 2) == 0 || ft_strncmp(cmd, "exit", 2) == 0 || ft_strncmp(cmd, "echo", 2) == 0
				|| ft_strncmp(cmd, "pwd", 2) == 0 || ft_strncmp(cmd, "export", 2) == 0 || ft_strncmp(cmd, "env", 2) == 0
					|| ft_strncmp(cmd, "unset", 2) == 0);
}

int check_if_single_builtin(t_cmd *cmd)
{
	t_execcmd	*ecmd;

	ecmd = (t_execcmd *)cmd;
	if (ecmd->argv[0] == 0)
		exit(127);
	// or better if (ecmd->type != EXEC && ecmd->type != Redir && is_buildin(ecmd->argv[0]))
	return (ecmd->type == EXEC && is_buildin(ecmd->argv[0]));
}

int run_single_builtin(t_cmd *cmd, t_args *params)
{
	t_execcmd	*ecmd;
	int	builtin_status;

	ecmd = (t_execcmd *)cmd;
	fprintf(stderr, "Running command: %s\n", ecmd->argv[0]); // Debug message
	builtin_status = run_buildin(ecmd, params);
	return (builtin_status);
}