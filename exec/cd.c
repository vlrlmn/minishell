/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:39:06 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/06/06 16:55:20 by sabdulki         ###   ########.fr       */
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

int cd_cmd(t_execcmd *ecmd, t_args *params) 
{
    char path[1024];
    (void)params;
    char *tmp_path;
    // char oldpath[1024] = "OLDPWD=";
    // char res_oldpath[1024];
    // char pwd[1024] = "PWD=";
    // char res_pwd[1024];
    // char *cur_path;
    char *new_path;

    // if (getcwd(path, sizeof(path)) != NULL)
    //     printf("Current working directory before: %s\n", path);
    
    if ((ecmd->argv[1] == NULL) || (strcmp(ecmd->argv[1], "~") == 0))
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
    {
        tmp_path = getcwd(path, sizeof(path));
        if (tmp_path == NULL)
            return (1); //ERROR todo
        // printf("tmp_path: %s\n", tmp_path);
        relative_path(path, ecmd);
    }
    if (chdir(path) != 0) 
    {
        perror("cd");
        return (1);
    }
    printf("oldpwd1: %s\n", find_env_var(params->envp, "OLDPWD"));
    printf("pwd1: %s\n", find_env_var(params->envp, "PWD"));
    
    update_oldpwd(ecmd, params, tmp_path);
    new_path = getcwd(path, sizeof(path));
    update_pwd(ecmd, params, new_path);

    printf("oldpwd2: %s\n", find_env_var(params->envp, "OLDPWD"));
    printf("pwd2: %s\n", find_env_var(params->envp, "PWD"));
    // if (getcwd(path, sizeof(path)) != NULL)
    //     printf("Current working directory after: %s\n", path);
    // else
    //     perror("getcwd() error");
    // printf("argv[1]: %s\npath:%s\n", ecmd->argv[1], )
    return (0);
}

// void update_envp_var(char flag, char *src)
// {
//     char res_str[1024];
//     char dest[1024];
//         if (flag == 'p')
//             dest = "PWD";
//     else if (flag == 'o')
//         cha
//     ft_strlcat(dest, src, sizeof(dest));
//     ft_strlcpy(res_str, dest, sizeof(dest));
//     putenv(res_str);
// }

int update_oldpwd(t_execcmd *ecmd, t_args *params, char *new_oldpwd_content)
{
    char    *oldpwd;
    int     index;

    (void)ecmd;
    oldpwd = ft_strjoin("PWD=", ft_strdup(new_oldpwd_content));
    if (!oldpwd)
        return (1);
    index = find_env_index(params->envp, "PWD");
    free(params->envp[index]);
    params->envp[index] = oldpwd;
    return (0);
    
    // char    *oldpwd_env_var;
    // int     len;

    // (void)ecmd;
    
    // oldpwd_env_var = find_env_var(params->envp, "OLDPWD");
    // if (!oldpwd_env_var)
    //     return(printf("No oldpwd in env vars!\n"), 1);
    // printf("oldpwd beforre updating: %s\n", oldpwd_env_var);
    // len = ft_strlen(oldpwd_env_var);
    // oldpwd_env_var = ft_memset(oldpwd_env_var, 0, len);
    // // oldpwd_env_var = 
    // ft_strlcpy(oldpwd_env_var, tmp_path, ft_strlen(tmp_path) + 1);
    // // установить в params->envp новое значение для pwd
    // printf("updated oldpwd: %s\n", oldpwd_env_var);
    // return (0);
}

int update_pwd(t_execcmd *ecmd, t_args *params, char *new_pwd_content)
{
    char    *pwd;
    int     index;

    (void)ecmd;
    pwd = ft_strjoin("PWD=", ft_strdup(new_pwd_content));
    if (!pwd)
        return (1);
    index = find_env_index(params->envp, "PWD");
    free(params->envp[index]);
    params->envp[index] = pwd;
    return (0);
}

int find_env_index(char **envp, char *var)
{
    int		i;
    int     len;
    char    *dest;

	i = 0;
    len = 0;
    if (ft_strncmp(var, "PATH", 4) == 0)
    {
        dest = "PATH";
        len = 4;
    }
    else if (ft_strncmp(var, "PWD", 3) == 0)
    {
        dest = "PWD";
        len = 3;
    }
    else if (ft_strncmp(var, "OLDPWD", 6) == 0)
    {
        dest = "OLDPWD";
        len = 6;
    }
    else
    {
        printf("Wrong envp variable!\n");
        return (1);
    }
	while (envp[i])
	{
		if (ft_strncmp(envp[i], dest, len) == 0)
		{
			// res = envp[i] + (len + 1);
			// res = envp[i];
			// break ;
            return (i);
		}
		i++;
	}
    printf("No such variable\n");
    return (1);
}

char    *find_env_var(char **envp, char *var)
{
	int		i;
    int     len;
	char	*res;
    char    *dest;

	i = 0;
    len = 0;
	res = NULL;
    if (ft_strncmp(var, "PATH", 4) == 0)
    {
        dest = "PATH";
        len = 4;
    }
    else if (ft_strncmp(var, "PWD", 3) == 0)
    {
        dest = "PWD";
        len = 3;
    }
    else if (ft_strncmp(var, "OLDPWD", 6) == 0)
    {
        dest = "OLDPWD";
        len = 6;
    }
    else
    {
        printf("Wrong envp variable!\n");
        return (NULL);
    }
	while (envp[i])
	{
		if (ft_strncmp(envp[i], dest, len) == 0)
		{
			res = envp[i] + (len + 1);
			// res = envp[i];
            return (res);
		}
		i++;
	}
	return (NULL);
}