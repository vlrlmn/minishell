/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_exec_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aajaanan <aajaanan@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 19:17:15 by aajaanan          #+#    #+#             */
/*   Updated: 2023/09/01 19:21:30 by aajaanan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

void	remove_empty_args(t_execcmd *ecmd)
{
	int	i;

	while (ecmd->argv[0] && ecmd->argv[0][0] == '\0')
	{
		i = 0;
		while (ecmd->argv[i] && ecmd->argv[i + 1])
		{
			ecmd->argv[i] = ecmd->argv[i + 1];
			i++;
		}
		ecmd->argv[i] = NULL;
	}
}

void	handle_invalid_executable(t_execcmd *ecmd, t_params *params,
	struct stat path_stat)
{
	if (S_ISDIR(path_stat.st_mode))
	{
		ft_printf_fd(STDERR_FILENO, "minishell: %s: is a directory\n",
			ecmd->argv[0]);
		free_exit(params, 126);
	}
	else if (access(ecmd->argv[0], X_OK) != 0)
	{
		ft_printf_fd(STDERR_FILENO, "minishell: %s: Permission denied\n",
			ecmd->argv[0]);
		free_exit(params, 126);
	}
}

void	handle_executable_path(t_execcmd *ecmd, t_params *params)
{
	struct stat	path_stat;

	if (ecmd->argv[0] == NULL)
		free_exit(params, 0);
	else if (ft_strchr("./", ecmd->argv[0][0]))
	{
		if (stat(ecmd->argv[0], &path_stat) == 0)
			handle_invalid_executable(ecmd, params, path_stat);
		else
		{
			ft_printf_fd(STDERR_FILENO,
				"minishell: %s: No such file or directory\n",
				ecmd->argv[0]);
			free_exit(params, 127);
		}
	}
}

char	*find_command_path(char *cmd, char *path_var)
{
	int		i;
	char	**paths;
	char	*command;
	char	*binary_path;

	paths = ft_split(path_var, ':');
	command = ft_strjoin("/", cmd);
	i = 0;
	while (paths[i])
	{
		binary_path = ft_strjoin(paths[i], command);
		if (access(binary_path, X_OK) == 0)
		{
			free1(command);
			free_split(paths);
			return (binary_path);
		}
		free1(binary_path);
		i++;
	}
	free1(command);
	free_split(paths);
	return (NULL);
}
