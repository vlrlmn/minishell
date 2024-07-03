/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expantion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 14:19:33 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/03 18:05:30 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_add_exp(char *s1, char *s2)
{
	if (s1)
		free(s1);
	if (s2)
		free(s2);
}

int	is_expantion(char *input)
{
	size_t	i;

	i = 0;
	while (input[i] && input[i] != '$')
		i++;
	if (i == (ft_strlen(input))) //if there are no '$'
	{
		// fprintf(stderr, "no expantions!\n");
		return (0);
	}
	// fprintf(stderr, "found expantion!\n");
	return (1);
}

char	*add_expantion(char *input, t_args *args)
{
    size_t	i;
	size_t	k;
	char	*var_value;
	char	*env_var;
	char	*res;
	char	*input_before_exp;
	int		middle;
	char	*rem;
	int		len;

	i = 0;
	k = 0;
	middle = 0;
	// res = NULL;
	env_var = NULL;
	var_value = NULL;
	input_before_exp = NULL;
	while (input[i] && input[i] != '$') //find the posostion of '$'
		i++;
	if (is_delimiter(input[i + 1]) || input[i + 1] == '\0')
		return (input);
		// return (fprintf(stderr, "no expantions, only '$' sign!\n"), input);
	k = i;
	k += 1;
	//check if there are some chars after exp. example: '$HOME 123'
	while (input[k] && (input[k] != '$' && !is_delimiter(input[k])))
		k++;
	if (k != ft_strlen(input)) //if there are chars after '$EXP  smth'
		middle = 1;
	env_var = get_str_after_sign(input, '$'); 				//free DONE
	// fprintf(stderr, "env_var: '%s'\n", env_var);
    var_value = find_env_var(args->envp, env_var);
    // printf("var_value: %s\n", var_value);
	input_before_exp = get_str_before_sign(input, '$');		//free DONE
	// printf("input_before_exp: %s\n", input_before_exp);
	res = ft_strjoin(input_before_exp, var_value); // return input + meaning of exp free
	free_add_exp(env_var, input_before_exp); //or input
	if (middle == 1 && res)
	{
		len = ft_strlen(input) - k;
		rem = malloc(sizeof(char) * (len + 1)); //free
		if (!rem)
			return (free(res), NULL);
		ft_strlcpy(rem, &input[k], ft_strlen(rem));
		if (is_expantion(rem))
			rem = add_expantion(rem, args); //rem becomes input
		res = ft_strjoin(res, rem);
		free(rem);
	}
	fprintf(stderr, "result_str: '%s'\n", res);
    return (res);
}
