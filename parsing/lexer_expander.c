/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_expander.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:34:57 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/03 15:32:15 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*getvar_name(char *arg)
{
	int	i;
	int	n;

	i = 0;
	if (ft_isdigit(arg[i]))
	{
		n = arg[i] - '0';
		return (ft_itoa(n));
	}
	while (arg[i] && ((ft_isalnum(arg[i]) || arg[i] == '_')))
		i++;
	return (ft_substr(arg, 0, i));
}

void parse_expander_sign(int *i, char *line, t_lexems *list, t_args *args)
{
    char	*var_name;
	char	*var_value;
	int		*j;

	j = i;
	// (*j)++;
	var_name = getvar_name(line + *j);
	var_value = get_env(var_name, args->envp);
	if (var_value)
	{
		var_value += ft_strlen(var_name) + 1;
		add_str_node(list, var_value);
	}
	// else
	// 	add_str_node(list, var_name);
	(*j) += ft_strlen(var_name);
	free(var_name);
}

void parse_expander_sign_in_quotes(int *i, char *line, t_lexems *list, t_args *args)
{
    char	*var_name;
	char	*var_value;
	int		*j;

	j = i;
	(*j)++;
	var_name = getvar_name(line + *j);
	var_value = get_env(var_name, args->envp);
	if (var_value)
	{
		var_value += ft_strlen(var_name) + 1;
		add_str_node(list, var_value);
	}
	(*j) += ft_strlen(var_name);
	free(var_name);
}

void parse_expander(int *i, t_lexems *list, char *line, t_args *args)
{
    char *exit_str;

    (*i)++;
    if(!line[*i] || is_delimiter(line[*i]))
    {
        add_char_node(list, '$');
        return ;
    }
    else if (line[*i] == '?')
    {
        exit_str = ft_itoa(g_exit_status);
        add_str_node(list, exit_str);
        free(exit_str);
        (*i)++;
    }
    else
        parse_expander_sign(i, line, list, args);
}