/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_expander.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:34:57 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/14 12:17:04 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

char	*get_env_exp(char *value, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strlen(value) && (ft_strncmp(value, envp[i],
					ft_strlen(value)) == 0) && envp[i][ft_strlen(value)] == '=')
			return (envp[i]);
		i++;
	}
	return (NULL);
}

void	parse_expander_sign(int *i, char *line, t_lexems *list, t_args *args)
{
	char	*var_name;
	char	*var_value;
	int		*j;

	j = i;
	var_name = getvar_name(line + *j);
	var_value = get_env_exp(var_name, args->envp);
	if (var_value)
	{
		var_value += ft_strlen(var_name) + 1;
		add_str_node(list, var_value);
	}
	(*j) += ft_strlen(var_name);
	free(var_name);
}

void	parse_expander_sign_in_quotes(int *i, char *line, t_lexems *list,
		t_args *args)
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

void	parse_expander(int *i, t_clean_line_args args, int *exit_status)
{
	char	*exit_str;

	(*i)++;
	if (!args.line[*i] || is_delimiter(args.line[*i]) || (args.line[*i + 1]
			&& args.line[*i] == '/' && args.line[*i + 1] == '/'))
	{
		add_char_node(args.list, '$');
		return ;
	}
	else if (args.line[*i] == '?')
	{
		exit_str = ft_itoa(*exit_status);
		add_str_node(args.list, exit_str);
		free(exit_str);
		(*i)++;
	}
	else if (args.line[*i] == '0')
	{
		add_str_node(args.list, "bash\n");
		(*i)++;
	}
	else
		parse_expander_sign(i, args.line, args.list, args.args);
}
