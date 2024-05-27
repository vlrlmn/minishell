/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:39:06 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/05/27 15:46:15 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void relative_path(char *result, t_execcmd *ecmd) 
{
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) == NULL) 
    {
        perror("getcwd");
        return;
    }
    ft_strlcat(cwd, "/", sizeof(cwd));
    ft_strlcat(cwd, ecmd->argv[1], sizeof(cwd));
    ft_strlcpy(result, cwd, sizeof(cwd));
}

int cd_cmd(t_execcmd *ecmd) 
{
    char path[1024];

    if (ecmd->argv[1] == NULL || strcmp(ecmd->argv[1], "~") == 0) 
    {
        char *home = getenv("HOME");
        if (!home) 
        {
            printf("cd: HOME not set\n");
            return (1);
        }
        ft_strlcpy(path, home, sizeof(path) - 1);
        path[sizeof(path) - 1] = '\0';
    } 
    else if (ecmd->argv[1][0] == '/') 
    {
        ft_strlcpy(path, ecmd->argv[1], sizeof(path) - 1);
        path[sizeof(path) - 1] = '\0';
    } 
    else
        relative_path(path, ecmd);
    if (chdir(path) != 0) 
    {
        perror("cd");
        return (1);
    }
    return (0);
}
