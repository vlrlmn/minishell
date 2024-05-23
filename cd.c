/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:39:06 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/05/23 16:11:03 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_env_value (char *name, char **envp)
{
    size_t len;
    int i;

    len = ft_strlen(name);
    i = 0;
    while(envp[i])
    {
        if(ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
            return (envp[i] + len + 1);
        i++;
    }
}

void *relative_path(char *path, t_execcmd *ecmd)
{
    char cwd[1024];
    
}

int cd_cmd(t_execcmd *ecmd, t_args *params)
{
    char *path;

    path = NULL;
    if(ecmd->argv[1] == NULL || ft_strncmp(ecmd->argv[1], "~", 1) == 0)
    {
        path = get_env_value("HOME", params->envp);
        if (!path)
        {
            exit_with_err("cd: HOME not set\n");
            return (1);
        }
    }
    else if (ecmd->argv[1][0] == '/')
        path = ecmd->argv[1];
    else
        relative_path(&path, ecmd);
    if (chdir(path))
    {
        perror ("cd");
        return (1);
    }
    return (0);
}