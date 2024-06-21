/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expantion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 14:19:33 by sabdulki          #+#    #+#             */
/*   Updated: 2024/06/21 21:44:55 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_null(char *s1, char *s2, char *s3, char *s4)
{
	s1 = NULL;
	s2 = NULL;
	s3 = NULL;
	s4 = NULL;
}

void	free_add_exp(char *s1, char *s2, char *s3)
{
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	if (s3)
		free(s3);
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
		return (fprintf(stderr, "no expantions, only '$' sign!\n"), input);
	k = i;
	k += 1;
	//check if there are some chars after exp. example: '$HOME 123'
	while (input[k] && (input[k] != '$' && !is_delimiter(input[k])))
		k++;
	if (k != ft_strlen(input)) //if there are chars after '$EXP  smth'
		middle = 1;
	env_var = get_str_after_sign(input, '$');
	// fprintf(stderr, "env_var: '%s'\n", env_var);
    var_value = find_env_var(args->envp, env_var);
    // printf("var_value: %s\n", var_value);
	input_before_exp = get_str_before_sign(input, '$');
	// printf("input_before_exp: %s\n", input_before_exp);
	res = ft_strjoin(input_before_exp, var_value); // return input + meaning of exp
	free_add_exp(var_value, env_var, input_before_exp); //or input
	if (middle == 1 && res)
	{
		len = ft_strlen(input) - k;
		rem = malloc(sizeof(char) * (len + 1));
		ft_strlcpy(rem, &input[k], ft_strlen(rem));
		if (is_expantion(rem))
			rem = add_expantion(rem, args);
		res = ft_strjoin(res, rem);
		free(rem);
	}
	fprintf(stderr, "result_str: '%s'\n", res);
    return (res);
}
