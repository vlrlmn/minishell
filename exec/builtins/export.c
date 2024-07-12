/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 17:48:27 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/12 16:29:28 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	pre_export_cmd(t_cmd_info *ecmd, t_args *params)
{
	int	i;
	int	status;

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

int	export_print(t_args *params)
{
	int		i;
	char	*value;
	char	*name;

	i = 0;
	while (params->envp[i])
	{
		// printf("str: '%s'\n", params->envp[i]);
		value = get_str_after_sign(params->envp[i], '=');
		name = get_str_before_sign(params->envp[i], '=');
		if (value == NULL || (ft_strncmp(name, "OLDPWD", ft_strlen(name)) == 0
				&& value[0] == '\0'))
			printf("declare -x %s\n", name);
		else
			printf("declare -x %s=\"%s\"\n", name, value);
		free(name);
		free(value);
		i++;
	}
	return (0);
}

int	export_cmd(char *str, t_args *params)
{
    char    *env_var;
    char    *env_value;
    int i = 0;
    
    if (!str)
        return (printf("export: invalid argument\n"), 1);
    env_var = get_str_before_sign(str, '=');
    while(env_var[i] == '-')
    {
        i++;
        if (ft_isalnum(env_var[i]))
        {
            printf("bash: export: invalid option export: usage: export [-nf] [name[=value] ...] or export -p\n"); 
            free(env_var);
            return (2);
        }
    }
    if (!is_valid_variable_name(env_var))
    {
        printf("export: '%s': not a valid identifier\n", env_var);
        free(env_var);
        return (1);
    }
    free(env_var);
    // if (ft_isdigit(env_var[0]))
    //     return (printf("export: '%s': not a valid identifier\n", env_var), 1);
    env_value = get_str_after_sign(str, '=');
    if (find_env_var(params->envp, env_var))
    {
        update_envp_var(params, env_var, env_value);
    }
    else
        add_cmd(params, str);
    // free(env_var);
    free(env_value);
    return (0);
}

void	free_arr(char **new_env_list, int i)
{
	int	j;

	j = -1;
	while (++j < i)
		free(new_env_list[j]);
	free(new_env_list);
}

int	add_cmd(t_args *params, char *new_env_var)
{
	int		size;
	int		i;
	char	**new_env_list;

	i = -1;
	size = 0;
	while (params->envp[size])
		size++;
	new_env_list = malloc(sizeof(char *) * (size + 2));
	if (!new_env_list)
		return (1);
	while (++i < size)
		new_env_list[i] = params->envp[i];
	new_env_list[i] = ft_strdup(new_env_var);
	if (!new_env_list[i])
		return (free_arr(new_env_list, i), 1);
	i += 1;
	new_env_list[i] = NULL;
	free(params->envp);
	params->envp = new_env_list;
	return (0);
}
