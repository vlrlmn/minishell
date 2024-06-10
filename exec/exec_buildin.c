/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_buildin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:53:53 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/06/10 17:14:12 by sabdulki         ###   ########.fr       */
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
        printf("%s\n", ecmd->argv[i]);
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