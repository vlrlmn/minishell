/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_buildin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:53:53 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/05/22 14:59:43 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int run_buildin(t_execcmd *ecmd, t_args *params)
{
    int status;

    status = -1;
    if(ft_strncmp(ecmd->argv[0], "cd", 3) == 0)
        cd_cmd(ecmd, params);
    else if(ft_strncmp(ecmd->argv[0], "exit", 5) == 0)
        exit_cmd();
    else if (ft_strncmp(ecmd->argv[0], "echo", 5) == 0)
        echo_cmd();
    else if(ft_strncmp(ecmd->argv[0], "pwd", 4) == 0)
        pwd_cmd();
    else if(ft_strncmp(ecmd->argv[0], "export", 7) == 0)
        export_cmd();
    else if (ft_strncmp(ecmd->argv[0], "unset", 6) == 0)
        unset_cmd();
    else if(ft_strncmp(ecmd->argv[0], "env", 4) == 0)
        env_cmd();
    return(status);
}