/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 17:30:44 by vlomakin          #+#    #+#             */
/*   Updated: 2024/07/13 20:39:14 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*find_cmd_path(char *cmd, char *path)
{
	char	*cmd_path;
	char	**path_arr;
	char	*command;
	int		i;

	i = 0;
	if (!path)
		return (NULL);
	if (ft_strnstr(cmd, "./", ft_strlen(cmd)) || ft_strnstr(cmd, "../",
			ft_strlen(cmd)))
		return (cmd);
	path_arr = ft_split(path, ':');
	command = ft_strjoin("/", cmd);
	while (path_arr[i])
	{
		cmd_path = ft_strjoin(path_arr[i], command);
		if (access(cmd_path, X_OK) == 0)
			return (free(command), free_split(path_arr), cmd_path);
		free(cmd_path);
		i++;
	}
	free(command);
	free_split(path_arr);
	return (NULL);
}

int	if_path_to_cmd(char *path_line)
{
	if (access(path_line, F_OK) == 0 && access(path_line, X_OK) == 0)
		return (0);
	return (1);
}

int	is_executable(const char *path)
{
	struct stat	sb;

	if (stat(path, &sb) == 0 && S_ISREG(sb.st_mode))
	{
		if (access(path, X_OK) == 0)
		{
			return (0);
		}
	}
	return (1);
}
