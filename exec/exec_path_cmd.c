/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:53:49 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/03 17:37:24 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *find_command_path(char *cmd, char *path)
{
    char *cmd_path = NULL;
    char **path_arr;
    char *command;
    int i;

    i = 0;
    if (!path)
        return (NULL);
    // if cmd contains ./ or ../ -> it's a path.
    if (ft_strnstr(cmd, "./", ft_strlen(cmd)) || ft_strnstr(cmd, "../", ft_strlen(cmd)))
    {
        return (cmd);
    }
    path_arr = ft_split(path, ':');
    command = ft_strjoin("/", cmd);
    while (path_arr[i])
    {
        cmd_path = ft_strjoin(path_arr[i], command);
        if (access (cmd_path, X_OK) == 0)
        {
            free_split(path_arr);
            return (cmd_path);
        }
        free(cmd_path);
        i++;
    }
    free(command);
    free_split(path_arr);
    return (NULL);
}
