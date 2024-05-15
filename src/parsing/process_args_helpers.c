/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_args_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aajaanan <aajaanan@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 11:59:10 by aajaanan          #+#    #+#             */
/*   Updated: 2023/09/02 12:09:21 by aajaanan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parsing.h"

int	contains_special_char(char *arg)
{
	if (ft_strchr(arg, '\'') || ft_strchr(arg, '\"') || ft_strchr(arg, '$')
		|| ft_strchr(arg, '~'))
		return (1);
	return (0);
}

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

void	process_single_quote(char *arg, int *i, t_queue_char *q)
{
	(*i)++;
	while (arg[*i] && arg[*i] != '\'')
	{
		enqueue_char(q, arg[*i]);
		(*i)++;
	}
	(*i)++;
}
