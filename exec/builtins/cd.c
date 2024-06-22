/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:39:06 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/06/22 17:05:34 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// TODO errors handling EVERYWHERE where return(1);

void    relative_path(char *result, t_cmd_info *ecmd) 
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

int cd_cmd(t_cmd_info *ecmd, t_args *params) 
{
    char path[1024];
    char *old_path;
    char *new_path;
    char oldpwd[1024] = "OLDPWD";
    char pwd[1024] = "PWD";

    old_path = getcwd(path, sizeof(path));
    if (!old_path)
        return (1); // TODO errors handling
    old_path = ft_strdup(old_path);
    if (old_path == NULL)
        return (1); //ERROR todo
    if ((ecmd->argv[1] == NULL) || (strcmp(ecmd->argv[1], "~") == 0)) //forbidden func!
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
    update_envp_var(params, oldpwd, old_path); //what if result is 1? handle error
    new_path = getcwd(path, sizeof(path));
    if (!new_path)
        return (1);
    update_envp_var(params, pwd, new_path);
    // printf("list oldpwd: %s\n", find_env_var(params->envp, "OLDPWD"));
    // printf("list pwd: \t%s\n", find_env_var(params->envp, "PWD"));
    return (0);
}
