/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 17:46:04 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/05/29 16:43:01 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int pwd_cmd(t_execcmd *ecmd)
{
    char cwd[1024];

    (void)ecmd;
    if(getcwd(cwd, sizeof(cwd)))
        printf("%s\n", cwd);
    else
    {
        perror("pwd");
        return (1);
    }
    return (0);
}