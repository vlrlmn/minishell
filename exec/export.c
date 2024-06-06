/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 17:48:27 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/06/06 18:25:05 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//REWRITE FUNCTION
int set_env(const char *name, const char *value, t_args *args) 
{
    size_t len = ft_strlen(name);
    int i;

    i = 0;
    for (i = 0; args->envp[i] != NULL; i++) {
        if (strncmp(args->envp[i], name, len) == 0 && args->envp[i][len] == '=') 
        {
            free(args->envp[i]);
            args->envp[i] = malloc(len + strlen(value) + 2);
            if (args->envp[i] == NULL) {
                perror("malloc");
                return 1;
            }
            sprintf(args->envp[i], "%s=%s", name, value);
            return 0;
        }
    }
    args->envp[i] = malloc(len + strlen(value) + 2);
    if (args->envp[i] == NULL) {
        perror("malloc");
        return 1;
    }
    sprintf(args->envp[i], "%s=%s", name, value);
    args->envp[i + 1] = NULL;
    return 0;
}
// REWRITE FUNCTION


int export_cmd(t_execcmd *ecmd, t_args *params)
{
    char *equal_sign;

    if (ecmd->argv[1])
    {
        equal_sign = ft_strchr(ecmd->argv[1], '=');
        if (equal_sign)
        {
            equal_sign = "\0";
            set_env(ecmd->argv[1], equal_sign + 1, params); //TO_DO
            equal_sign = "=";
        }
        else
        {
           printf("export: invalid argument\n");
           return (1);
        }
    }
    else
    {
        printf("export: invalid argument\n"); // free memory where necessary
        return(1);
    }
    return (0);
}