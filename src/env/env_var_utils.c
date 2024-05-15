/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aajaanan <aajaanan@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 12:23:23 by aajaanan          #+#    #+#             */
/*   Updated: 2023/09/02 17:08:41 by aajaanan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/env.h"
#include "../../include/minishell.h"

void	free_env_var_node(t_env_var *node)
{
	if (node->key)
		free1(node->key);
	if (node->value)
		free1(node->value);
	free1(node);
}

void	free_env_var_list(t_env_var *env_var_list)
{
	t_env_var	*tmp;
	t_env_var	*current;

	current = env_var_list;
	while (current)
	{
		tmp = current->next;
		free_env_var_node(current);
		current = tmp;
	}
}

char	*getenv_value(char *key, t_env_var *env_var_list)
{
	t_env_var	*tmp;

	tmp = env_var_list;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}
