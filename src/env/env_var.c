/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aajaanan <aajaanan@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 10:04:49 by aajaanan          #+#    #+#             */
/*   Updated: 2023/09/04 09:39:19 by aajaanan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/env.h"
#include "../../include/minishell.h"

t_env_var	*env_var_new(char *key, char *value)
{
	t_env_var	*env_var_node;

	env_var_node = (t_env_var *)malloc(sizeof(t_env_var));
	if (!env_var_node)
		return (NULL);
	env_var_node->key = key;
	env_var_node->value = value;
	env_var_node->next = NULL;
	return (env_var_node);
}

int	env_var_update_value(t_env_var *env_var_list, t_env_var *new_nod)
{
	t_env_var	*tmp;

	tmp = env_var_list;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, new_nod->key) == 0)
		{
			if (new_nod->value == NULL)
			{
				free_env_var_node(new_nod);
				return (1);
			}
			free(tmp->value);
			tmp->value = ft_strdup(new_nod->value);
			free_env_var_node(new_nod);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	env_var_insert_sorted(t_env_var **env_var_list, t_env_var *new_node)
{
	t_env_var	*p;
	t_env_var	*q;

	p = *env_var_list;
	q = NULL;
	if (env_var_update_value(*env_var_list, new_node))
		return ;
	if (*env_var_list == NULL)
	{
		*env_var_list = new_node;
		return ;
	}
	if (ft_strcmp(new_node->key, (*env_var_list)->key) < 0)
	{
		new_node->next = *env_var_list;
		*env_var_list = new_node;
		return ;
	}
	while (p && ft_strcmp(new_node->key, p->key) > 0)
	{
		q = p;
		p = p->next;
	}
	q->next = new_node;
	new_node->next = p;
}

static void	env_var_add(char **envp, int i, t_env_var **env_var_list)
{
	char		*key;
	char		*value;
	char		*equal_sign;
	t_env_var	*new_node;

	equal_sign = ft_strchr(envp[i], '=');
	if (equal_sign)
	{
		key = ft_substr(envp[i], 0, equal_sign - envp[i]);
		value = ft_strdup(equal_sign + 1);
		if (ft_strcmp(key, "OLDPWD") == 0)
		{
			new_node = env_var_new(key, NULL);
			free(value);
		}
		else
			new_node = env_var_new(key, value);
	}
	else
		new_node = env_var_new(ft_strdup(envp[i]), NULL);
	env_var_insert_sorted(env_var_list, new_node);
}

void	init_env_var_list(char **envp, t_env_var **env_var_list)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		env_var_add(envp, i, env_var_list);
		i++;
	}
}
