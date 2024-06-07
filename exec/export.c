/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 17:48:27 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/06/06 20:34:05 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//REWRITE FUNCTION
// int set_env(const char *name, const char *value, t_args *args) 
// {
//     size_t len = ft_strlen(name);
//     int i;

//     i = 0;
//     for (i = 0; args->envp[i] != NULL; i++) {
//         if (strncmp(args->envp[i], name, len) == 0 && args->envp[i][len] == '=') 
//         {
//             free(args->envp[i]);
//             args->envp[i] = malloc(len + strlen(value) + 2);
//             if (args->envp[i] == NULL) {
//                 perror("malloc");
//                 return 1;
//             }
//             sprintf(args->envp[i], "%s=%s", name, value);
//             return 0;
//         }
//     }
//     args->envp[i] = malloc(len + strlen(value) + 2);
//     if (args->envp[i] == NULL) {
//         perror("malloc");
//         return 1;
//     }
//     sprintf(args->envp[i], "%s=%s", name, value);
//     args->envp[i + 1] = NULL;
//     return 0;
// }
// REWRITE FUNCTION


int export_cmd(t_execcmd *ecmd, t_args *params)
{
    if (ecmd->argv[1])
    {
        if (ft_strchr(ecmd->argv[1], '='))
            add_cmd(params, ecmd->argv[1]);
    }
    else
    {
        printf("export: invalid argument\n"); // free memory where necessary
        return(1);
    }
    return (0);
}

int add_cmd(t_args *params, char *new_env_var)
{
    int size;
    int i;
    char **new_env_list;

    i = 0;
    size = 0;
    while (params->envp[size])
        size++;
    new_env_list = malloc(sizeof(char *) * (size + 2)); // 1 for exported env_var, 1 for NULL
    if (!new_env_list)
        return (1);
    while (i < size)
    {
        new_env_list[i] = params->envp[i];
        i++;
    }
    new_env_list[i] = ft_strdup(new_env_var);
    if (!new_env_list[i])
    {
        //free all
        return (1);
    }
    i += 1;
    new_env_list[i] = NULL;
    free(params->envp);
    params->envp = new_env_list;
    return (0);
}

// int export_cmd(t_execcmd *ecmd, t_args *params)
// {
//     char *equal_sign;

//     if (ecmd->argv[1])
//     {
//         equal_sign = ft_strchr(ecmd->argv[1], '=');
//         if (equal_sign)
//         {
//             equal_sign = "\0";
//             set_env(ecmd->argv[1], equal_sign + 1, params); //TO_DO
//             equal_sign = "=";
//         }
//         else
//         {
//            printf("export: invalid argument\n");
//            return (1);
//         }
//     }
//     else
//     {
//         printf("export: invalid argument\n"); // free memory where necessary
//         return(1);
//     }
//     return (0);
// }