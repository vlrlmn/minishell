/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aajaanan <aajaanan@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 09:02:21 by aajaanan          #+#    #+#             */
/*   Updated: 2023/09/06 07:31:52 by aajaanan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"
#include "../../include/minishell.h"

static void	update_pwd(t_env_var *env_var_list)
{
	char		*pwd;
	t_env_var	*tmp;
	t_env_var	*old_pwd_node;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		ft_printf_fd(2, "minishell: cd: error retrieving current directory: \
getcwd: cannot access parent directories: No such file or directory\n");
		return ;
	}
	tmp = env_var_list;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "PWD") == 0)
		{
			old_pwd_node = env_var_new(ft_strdup("OLDPWD"), tmp->value);
			env_var_insert_sorted(&env_var_list, old_pwd_node);
			tmp->value = ft_strdup(pwd);
			free(pwd);
			return ;
		}
		tmp = tmp->next;
	}
}

static void	change_to_home_directory(int *exit_status,
	t_env_var *env_var_list)
{
	char	*home_dir;

	home_dir = getenv_value("HOME", env_var_list);
	if (home_dir)
	{
		if (chdir(home_dir) != 0)
		{
			perror("cd");
			*exit_status = 1;
		}
	}
	else
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		*exit_status = 1;
	}
}

static void	cd_oldpwd_check(t_env_var *env_var_list, int *exit_status)
{
	if (chdir(getenv_value("OLDPWD", env_var_list)) != 0)
	{
		ft_printf_fd(2, "minishell: cd: OLDPWD not set\n");
		*exit_status = 1;
	}
	else
	{
		ft_printf("%s\n", getenv_value("OLDPWD", env_var_list));
		update_pwd(env_var_list);
	}
}

static void	cd_path_check(char **argv, int *exit_status,
	t_env_var *env_var_list)
{
	char	*path;

	path = ft_strjoin(getenv_value("HOME", env_var_list), argv[1] + 1);
	if (chdir(path) != 0)
	{
		ft_printf_fd(2, "minishell: cd: %s: No such file or directory\n", path);
		*exit_status = 1;
	}
	else
		update_pwd(env_var_list);
	free(path);
}

void	cd(char **argv, int *exit_status, t_env_var *env_var_list)
{
	if (argv[1] == NULL)
	{
		change_to_home_directory(exit_status, env_var_list);
		update_pwd(env_var_list);
		return ;
	}
	if (ft_strcmp(argv[1], "-") == 0)
		cd_oldpwd_check(env_var_list, exit_status);
	else if (argv[1][0] == '~')
		cd_path_check(argv, exit_status, env_var_list);
	else if (chdir(argv[1]) != 0)
	{
		ft_printf_fd(2, "minishell: cd: %s: No such file or directory\n",
			argv[1]);
		*exit_status = 1;
	}
	else
		update_pwd(env_var_list);
}
