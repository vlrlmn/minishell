/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expantion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 14:19:33 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/12 00:26:13 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_expantion(char *input)
{
	size_t	i;

	i = 0;
	while (input[i] && input[i] != '$')
		i++;
	if (i == (ft_strlen(input)))
		return (0);
	return (1);
}

char	*join_input_and_value(char *input, t_args *args)
{
	char	*res;
	char	*env_name;
	char	*env_value;
	char	*input_before_exp;

	env_name = get_str_after_sign(input, '$'); 
	env_value = find_env_var(args->envp, env_name);		
	free(env_name);
	input_before_exp = get_str_before_sign(input, '$');
	if (input_before_exp && env_value)
	{
		res = ft_strjoin(input_before_exp, env_value);
		free(input_before_exp);
	}
	else if (!input_before_exp && !env_value)
		res = NULL;
	else if (!input_before_exp)
		res = ft_strdup(env_value);
	else //if (!env_value)
		res = input_before_exp;
	return (res);
}

char	*more_expantions(char *input, int k, char *old_res, t_args *args)
{
	int		len;
	char	*rem;
	char	*new_res;
	char	*new_rem;
	
	new_rem = NULL;
	len = ft_strlen(input) - k;
	rem = malloc(sizeof(char) * (len + 1));
	if (!rem)
		return (free(old_res), NULL);
	ft_strlcpy(rem, &input[k], ft_strlen(rem));
	if (is_expantion(rem))
		new_rem = add_expantion(rem, args); //rem becomes input
	if (new_rem)
	{
		free(rem);
		rem = new_rem;
	}
	new_res = ft_strjoin(old_res, rem);
	free(rem);
	free(old_res);
	return (new_res);
}

int	check_dollar_sign(char *input)
{
	int	k;

	k = 0;
	while (input[k] && input[k] != '$')
		k++;
	if (is_delimiter(input[k + 1]) || input[k + 1] == '\0')
		return (0);
	k += 1;
	while (input[k] && (input[k] != '$' && !is_delimiter(input[k])))
		k++;
	return (k);
}

char	*add_expantion(char *input, t_args *args)
{
	size_t	k;
	char	*res;
	int		middle;

	middle = 0;
	k = check_dollar_sign(input);
	if (!k)
		return (input);
	if (k != ft_strlen(input))
		middle = 1;
	res = join_input_and_value(input, args);
	if (middle == 1 && res)
		res = more_expantions(input, k, res, args);
	fprintf(stderr, "result_str: '%s'\n", res);
    return (res);
}
