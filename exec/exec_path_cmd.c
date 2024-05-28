/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:53:49 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/05/28 18:10:10 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *find_command_path(char *cmd, char *path)
{
    char *cmd_path;
    char **path_arr;
    char *command;
    int i;

    i = 0;
    path_arr = ft_split(path, ':');
    command = ft_strjoin("/", cmd);
    while(path_arr[i])
    {
        cmd_path = ft_strjoin(path_arr[i], command);
        if (access (cmd_path, X_OK) == 0)
        {
            free(cmd);
            free(path_arr);
            return (cmd_path);
        }
        free(cmd_path);
        i++;
    }
    free(command);
    free(path_arr);
    return NULL;
}
