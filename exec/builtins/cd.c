/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:39:06 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/09 22:20:42 by sabdulki         ###   ########.fr       */
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

// char	*define_directory(char *path, t_cmd_info *ecmd, t_args *params)
int	define_directory(t_cmd_info *ecmd, t_args *params)
{
	char	*home;
	char	*prevdir;
	char	path[1024];

	if ((!ecmd->argv[1]) || (ft_strncmp(ecmd->argv[1], "HOME=", 5) == 0))
	{
		if (ecmd->argv[1] == NULL)
			ecmd->argv[1] = get_env("HOME=", params->envp);
		home = get_str_after_sign(ecmd->argv[1], '=');
		if (!home)
			return (printf("cd: HOME not set\n"), 1);
		ft_strlcpy(path, home, sizeof(path));
		free(home);
	}
	else if (ecmd->argv[1][0] == '/')
	{
		ft_strlcpy(path, ecmd->argv[1], sizeof(path));
	}
	else if (ecmd->argv[1][0] == '-') // go to oldpwd, oldpwd becomes current direcrory
	{
		prevdir = find_env_var(params->envp, "OLDPWD"); // attention
		if (prevdir[0] == '\0')
			return (printf("cd: OLDPWD not set\n"), 1);
		printf("'%s'\n", prevdir); // don't delete this printf,	it's needed for bash replication
		ft_strlcpy(path, prevdir, sizeof(path));
	}
	else
		relative_path(path, ecmd);
	return (0);
	// return (path);
}

int	cd_cmd(t_cmd_info *ecmd, t_args *params)
{
	// char	path[1024];
	char buffer[1024];
	char *path = buffer;
	char	*old_path;
	char	*new_path;
	char	*oldpwd;
	char	*pwd;

	old_path = ft_strdup(getcwd(path, sizeof(path)));
	if (!old_path)
		return (1);
	// define_directory(path, ecmd, params);
	if (define_directory(ecmd, params))
		return (free(old_path), 1);
	if (chdir(path) != 0)
		return (perror("cd"), 1);
	oldpwd = ft_strdup("OLDPWD");
	if (!oldpwd)
		return (free(old_path), 1);
	if (find_env_index(params->envp, "PWD") != -1)
		update_envp_var(params, oldpwd, old_path);
	free(old_path);
	free(oldpwd);
	new_path = getcwd(path, sizeof(path));
	if (!new_path)
		return (1);
	pwd = ft_strdup("PWD");
	if (!pwd)
		return (free(oldpwd), 1);
	update_envp_var(params, pwd, new_path);
	free(pwd);
	return (0);
}
