/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 17:46:04 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/05/31 15:55:48 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//after cd have to update pwd !!
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