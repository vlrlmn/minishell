/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:39:06 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/10 13:49:58 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// TODO errors handling EVERYWHERE where return(1);

void	relative_path(char *result, t_cmd_info *ecmd)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		return ;
	}
	ft_strlcat(cwd, "/", sizeof(cwd));
	ft_strlcat(cwd, ecmd->argv[1], sizeof(cwd));
	ft_strlcpy(result, cwd, sizeof(cwd));
}

char	*define_prevdir(t_args *params)
{
	char	*prevdir;

	prevdir = find_env_var(params->envp, "OLDPWD");
	if (prevdir == NULL || prevdir[0] == '\0')
		return (printf("cd: OLDPWD not set\n"), NULL);
	// don't delete this printf, it's needed for bash replication	
	printf("'%s'\n", prevdir);
	return (prevdir);
}

char	*define_directory(t_cmd_info *ecmd, t_args *params)
{
	char	*home;
	char	path[1024];

	if ((!ecmd->argv[1]) || ecmd->argv[1][0] == '\0' || \
		(ft_strncmp(ecmd->argv[1], "HOME=", 5) == 0))
	{
		if (ecmd->argv[1] == NULL || ecmd->argv[1][0] == '\0')
			ecmd->argv[1] = ft_strdup(get_env("HOME=", params->envp));
		home = get_str_after_sign(ecmd->argv[1], '=');
		if (!home)
			return (printf("cd: HOME not set\n"), NULL);
		ft_strlcpy(path, home, sizeof(path));
		free(home);
	}
	else if (ecmd->argv[1][0] == '/')
		ft_strlcpy(path, ecmd->argv[1], sizeof(path));
	else if (ecmd->argv[1][0] == '-')
	{
		if (!ft_strlcpy(path, define_prevdir(params), sizeof(path)))
			return (NULL);
	}
	else
		relative_path(path, ecmd);
	return (ft_strdup(path));
}

int	define_new_pwd(t_args *params)
{
	char	*pwd;
	char	buffer[1024];
	char	*new_path;

	new_path = getcwd(buffer, sizeof(buffer));
	if (!new_path)
		return (1);
	pwd = ft_strdup("PWD");
	if (!pwd)
		return (1);
	update_envp_var(params, pwd, new_path);
	free(pwd);
	return (0);
}

int	cd_cmd(t_cmd_info *ecmd, t_args *params)
{
	char	buffer[1024];
	char	*path;
	char	*old_path;
	char	*oldpwd;

	old_path = ft_strdup(getcwd(buffer, sizeof(buffer)));
	if (!old_path)
		return (1);
	path = define_directory(ecmd, params);
	if (!path)
		return (free(old_path), 1);
	if (chdir(path) != 0)
		return (perror("cd"), free(path), 1);
	free(path);
	oldpwd = ft_strdup("OLDPWD");
	if (!oldpwd)
		return (free(old_path), 1);
	if (find_env_index(params->envp, "PWD") != -1)
		update_envp_var(params, oldpwd, old_path);
	free(old_path);
	free(oldpwd);
	if (define_new_pwd(params))
		return (1);
	return (0);
}
