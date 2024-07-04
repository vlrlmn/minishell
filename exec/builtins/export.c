/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 17:48:27 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/04 13:49:40 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../minishell.h"

int pre_export_cmd(t_cmd_info *ecmd, t_args *params)
{
    int     i;
    int     status;

    i = 1;
    if (!ecmd->argv[i])
    {
        status = export_print(params);
        return (status);
    }
    while (ecmd->argv[i])
    {
        status = export_cmd(ecmd->argv[i], params);
        i++;
    }
    return (status);
}

int export_print(t_args *params)
{
    int     i;
	char	*value;
    char    *name;

	i = 0;
	while (params->envp[i])
	{
		//if value of variable is "" or value is NULL, do not print it.
		//should print if value contain only spaces (CHECK IN BASH 3.2)
		value = get_str_after_sign(params->envp[i], '=');
        name = get_str_before_sign(params->envp[i], '=');
		if (value == NULL || (ft_strncmp(name, "OLDPWD", ft_strlen(name)) == 0 && value[0] == '\0'))
            printf("declare -x %s\n", name);
        else
			printf("declare -x %s=\"%s\"\n", name, value);
        free(name);
		free(value);
		i++;
	}
	return (0);
}
int	is_valid_variable_name(char *key)
{
	int	i;

	i = 0;
	if (!ft_isalpha(key[i]) && key[i] != '_')
		return (0);
	i++;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
int export_cmd(char *str, t_args *params)
{
    char    *env_var;
    char    *env_value;
    
    if (!str)
        return (printf("export: invalid argument\n"), 1);
    env_var = get_str_before_sign(str, '=');
    if (!is_valid_variable_name(env_var))
    {
        printf("export: '%s': not a valid identifier\n", env_var);
        free(env_var);
        return (2);
    }
    // if (ft_isdigit(env_var[0]))
    //     return (printf("export: '%s': not a valid identifier\n", env_var), 1);
    env_value = get_str_after_sign(str, '=');
    if (find_env_var(params->envp, env_var))
    {
        update_envp_var(params, env_var, env_value);
    }
    else
        add_cmd(params, str);
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
    if (!new_env_list[i]) // Free all previously allocated memory
    {
        while (++j < i)
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
    int str_len;
    int len;
    int i;

    i = 0;
    str_len = 0;
    while (str[str_len])
        str_len++;
    while (str[i] != sign && i < str_len)
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
    if (!str)
        return NULL;
    while (str[i] != sign && str[i])
        i++;
    if (i == (ft_strlen(str))) //if there are no '$'
	{
		// fprintf(stderr, "no sign!\n");
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
