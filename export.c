/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 17:48:27 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/05/24 18:47:18 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int export_cmd(t_execcmd *ecmd, t_args *params)
{
    char *equal_sign;

    if(ecmd->argv[1])
    {
        equal_sign = ft_strchr(ecmd->argv[1], '=');
        if (equal_sign)
        {
            equal_sign = '\0';
            set_env(ecmd->argv[1], equal_sign + 1, params);
            equal_sign = '=';
        }
        else
            strerror ("export: invalid argument\n"); //or write??
    }
    else
        strerror ("export: invalid argument\n");
}