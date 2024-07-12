/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 17:30:44 by vlomakin          #+#    #+#             */
/*   Updated: 2024/07/12 00:27:23 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*find_command_path(char *cmd, char *path)
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
