/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:39:06 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/01 16:32:12 by sabdulki         ###   ########.fr       */
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
    char *prevdir;
    char *home;
    char    *home1;
    char oldpwd[1024] = "OLDPWD";
    char pwd[1024] = "PWD";

    home = NULL;
    old_path = getcwd(path, sizeof(path));
    if (!old_path)
        return (1); // TODO errors handling
    old_path = ft_strdup(old_path); //free
    if (old_path == NULL)
        return (1); //ERROR todo
    if ((!ecmd->argv[1]) || (ft_strncmp(ecmd->argv[1], "HOME", 4) == 0))
    {
        if (ecmd->argv[1] == NULL) //or if str is empty or contains only white spaces
            home = get_env("HOME", params->envp);
        home1 = get_str_after_sign(home, '=');
        if (!home1)
            return(printf("cd: HOME not set\n"), 1);
        ft_strlcpy(path, home1, sizeof(path));
        free(home1);
    }
    else if (ecmd->argv[1][0] == '/') 
    {
        ft_strlcpy(path, ecmd->argv[1], sizeof(path));
    }
    else if (ecmd->argv[1][0] == '-') //go to oldpwd, oldpwd becomes current direcrory
    {
        prevdir = find_env_var(params->envp, "OLDPWD"); //attention
        if (prevdir[0] == '\0')
            return (printf("cd: OLDPWD not set\n"), 1);
        printf("'%s'\n", prevdir); //don't delete this printf, it's needed for bash replication
        ft_strlcpy(path, prevdir, sizeof(path));
    }
    else
        relative_path(path, ecmd);
    if (chdir(path) != 0) 
    {
        perror("cd");
        return (1);
    }
    // if (find_env_index(params->envp, "OLDPWD") == -1)
    //     export_cmd("OLDPWD=", params);
    if (find_env_index(params->envp, "PWD") != -1)
        update_envp_var(params, oldpwd, old_path); //what if result is 1? handle error
    free(old_path);
    new_path = getcwd(path, sizeof(path));
    if (!new_path)
        return (1);
    update_envp_var(params, pwd, new_path);
    // printf("list oldpwd: %s\n", find_env_var(params->envp, "OLDPWD"));
    // printf("list pwd: \t%s\n", find_env_var(params->envp, "PWD"));
    return (0);
}
