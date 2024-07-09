/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 21:14:48 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/09 21:49:27 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

int	check_var_name(char *env_var)
{
	int	i;

	i = 0;
	while (env_var[i] == '-')
	{
		i++;
		if (ft_isalnum(env_var[i]))
		{
			printf("bash: line 1: export:-%c: invalid option \n \
			export: usage: export [-nf] [name[=value] ...] or export-p\n",
				env_var[i]);
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
	return (0);
}

char	*get_str_before_sign(char *str, char sign)
{
	char	*result;
	int		str_len;
	int		len;
	int		i;

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
	return (result);
}

char	*get_str_after_sign(char *str, char sign)
{
	char			*result;
	int				res_i;
	unsigned int	i;

	res_i = 0;
	i = 0;
	if (!str)
		return (NULL);
	while (str[i] != sign && str[i])
		i++;
	if (i == (ft_strlen(str)))
		return (NULL);
	result = (char *)malloc(sizeof(char) * ((ft_strlen(str) - i) + 1));
	if (!result)
		return (perror("malloc"), NULL);
	i += 1;
	while (i < ft_strlen(str) && str[i] != '\n')
	{
		result[res_i] = str[i];
		i++;
		res_i++;
	}
	result[res_i] = '\0';
	return (result);
}
