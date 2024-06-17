/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 17:46:04 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/06/17 17:30:22 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//after cd have to update pwd !!
int pwd_cmd(t_execcmd *ecmd, t_args *params)
{
    // char cwd[1024];
    char *pwd;

    (void)ecmd;
    
    // if (getcwd(cwd, sizeof(cwd)))
    //     printf("%s\n", cwd);
    pwd = find_env_var(params->envp, "PWD");
    if (pwd)
        printf("%s\n", pwd);
    else
    {
        perror("pwd");
        return (1);
    }
    return (0);
}