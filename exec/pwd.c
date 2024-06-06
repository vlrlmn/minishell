/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 17:46:04 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/06/05 20:39:44 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//after cd have to update pwd !!
int pwd_cmd(t_execcmd *ecmd)
{
    // char cwd[1024];
    char *res;

    (void)ecmd;
    res = getenv("PWD");
    if (res)
        printf("%s\n", res);
    // if (getcwd(cwd, sizeof(cwd)))
    //     printf("%s\n", cwd);
    else
    {
        perror("pwd");
        return (1);
    }
    return (0);
}