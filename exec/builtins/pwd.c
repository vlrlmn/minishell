/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 17:46:04 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/06/26 14:28:38 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//after cd have to update pwd !!
int pwd_cmd(t_cmd_info *ecmd, t_args *params)
{
    char cwd[1024];

    (void)ecmd;
    (void)params;
    if (getcwd(cwd, sizeof(cwd)))
        printf("%s\n", cwd);
    else
    {
        perror("pwd");
        return (1);
    }
    return (0);
}