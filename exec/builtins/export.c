/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 17:48:27 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/06/27 23:13:32 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int pre_export_cmd(t_cmd_info *ecmd, t_args *params)
{
    int     i;

    i = 1;
    if (!ecmd->argv[i])
    {
        export_print(params);
        return (0);
    }
    while (ecmd->argv[i])
    {
        export_cmd(ecmd->argv[i], params);
        i++;
    }
    return (0);
}

int export_print(t_args *params)
{
    int     i;
	char	*value;
    char    *before_sign;

	i = 0;
	while (params->envp[i])
	{
		//if value of variable is "", do not print it.
		//should print if value contain only spaces (CHECK IN BASH 3.2)
		value = get_str_after_sign(params->envp[i], '=');
        before_sign = get_str_before_sign(params->envp[i], '=');
		// if (value[0] == '\0')
            // printf("declare -x %s\n", before_sign);
        // else
			printf("declare -x %s=\"%s\"\n", before_sign, value);
        free(before_sign);
		free(value);
		i++;
	}
	return (0);
}

int export_cmd(char *str, t_args *params)
{
    char    *env_var;
    char    *env_value;
    
    if (!str)
        return (printf("export: invalid argument\n"), 1);
    printf("str: '%s'\n", str);
    env_var = get_str_before_sign(str, '=');
    if (ft_isdigit(env_var[0]))
        return (printf("export: '%s': not a valid identofier\n", env_var), 1);
    env_value = get_str_after_sign(str, '=');
    if (!env_value)
    {
        free(env_var);
        return (1);
    }
    if (find_env_var(params->envp, env_var))
    {
        update_envp_var(params, env_var, env_value);
        // printf("changed\n");
    }
    else if (ft_strchr(str, '='))
    {
        add_cmd(params, str);
        // printf("added new var!\n");
    }
    else
    {
        free(env_var);
        free(env_value);
        return (printf("export: invalid argument\n"), 1);
    }
    free(env_var);
    free(env_value);
    return (0);
}

int add_cmd(t_args *params, char *new_env_var)
{
    int size;
    int i;
    int j;
    char **new_env_list;

    i = 0;
    j = -1;
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
    if (!new_env_list[i]) {
        // Free all previously allocated memory
        while(++j < i)
            free(new_env_list[j]);
        free(new_env_list);
        return (1);
    }
    i += 1;
    new_env_list[i] = NULL;
    free(params->envp);
    params->envp = new_env_list;
    return (0);
}

char    *get_str_before_sign(char *str, char sign) 
{
    char *result;
    int len;
    int i;

    i = 0;
    while (str[i] != sign)
        i++;
    len = i;
    result = malloc(sizeof(char) * (len + 1));
    if (!result)
        return (perror("malloc"), NULL);
    ft_strlcpy(result, str, len + 1);
    // printf("res before equals: %s\n", result);
    return (result);
}

char    *get_str_after_sign(char *str, char sign)
{
    char *result;
    int len;
    unsigned int i;
    int res_i;

    res_i = 0;
    i = 0;
    while (str[i] != sign)
        i++;
    if (i == (ft_strlen(str))) //if there are no '$'
	{
		fprintf(stderr, "no sign!\n");
		return (NULL);
	}
    len = ft_strlen(str) - i;
    i += 1;
    result = (char *)malloc(sizeof(char) * (len + 1));
    if (!result)
        return (perror("malloc"), NULL);
    while (i < ft_strlen(str) && !is_delimiter(str[i]) && str[i] != sign)
	{
		result[res_i] = str[i];
		i++;
        res_i++;
	}
	result[res_i] = '\0';
    return (result);
}

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