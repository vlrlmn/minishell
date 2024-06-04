/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:39:06 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/06/04 17:21:58 by sabdulki         ###   ########.fr       */
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
    char *tmp_path;

    if (getcwd(path, sizeof(path)) != NULL)
        printf("Current working directory before: %s\n", path);
    
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
    tmp_path = getcwd(path, sizeof(path));
    if (tmp_path == NULL)
        return (1); //ERROR todo
    if (chdir(path) != 0) 
    {
        perror("cd");
        return (1);
    }
    update_oldpwd(ecmd, params, tmp_path);
    update_pwd(ecmd, params, path);
    if (getcwd(path, sizeof(path)) != NULL)
        printf("Current working directory after: %s\n", path);
    else
        perror("getcwd() error");
    // printf("argv[1]: %s\npath:%s\n", ecmd->argv[1], )
    return (0);
}

int update_oldpwd(t_execcmd *ecmd, t_args *params, char *tmp_path)
{
    char    *oldpwd_env_var;
    int     len;
    char    *res;

    oldpwd_env_var = find_env_var(params->envp, "OLDPWD");
    if (!oldpwd_env_var)
        return(printf("No oldpwd in env vars!\n"), 1);
    len = ft_strlen(oldpwd_env_var);
    oldpwd_env_var = ft_memset(oldpwd_env_var, 0, len);
    oldpwd_env_var = ft_strlcpy(oldpwd_env_var, tmp_path, (ft_strlen(tmp_path) - 1));
    oldpwd_env_var[ft_strlen(tmp_path) - 1] = '\0';
    // установить в params->envp новое значение для pwd
    printf("updated oldpwd: %s\n", oldpwd_env_var);
    return (0);
}

int update_pwd(t_execcmd *ecmd, t_args *params, char tmp_path)
{
    char *pwd_env_var;
    int     len;
    pwd_env_var = find_env_var(params->envp, "PWD");
    if (!pwd_env_var)
        return(printf("No pwd in env vars!\n"), 1);
    len = ft_strlen(pwd_env_var);
    pwd_env_var = ft_memset(pwd_env_var, 0, len);
    pwd_env_var = ft_strlcpy(pwd_env_var, tmp_path, (ft_strlen(tmp_path) - 1));
    pwd_env_var[ft_strlen(tmp_path) - 1] = '\0';
    // установить в params->envp новое значение для pwd
    printf("updated oldpwd: %s\n", pwd_env_var);
    return (0);
}

char	*find_env_var(char **envp, char *var)
{
	int		i;
    int     len;
	char	*res;
    char    *dest;

	i = 0;
    len = 0;
	res = NULL;
    if (ft_strncmp(var, "PATH", 4))
    {
        dest = "PATH";
        len = 4;
    }
    else if (ft_strncmp(var, "PWD", 3))
    {
        dest = "PWD";
        len = 3;
    }
    else if (ft_strncmp(var, "OLDPWD", 6))
    {
        dest = "OLDPWD";
        len = 6;
    }
	while (envp[i])
	{
		if (ft_strncmp(envp[i], dest, len))
		{
			res = envp[i] + (len + 1);
			break ;
		}
		i++;
	}
	if (res)
		return (res);
	return (NULL);
}