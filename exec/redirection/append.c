/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 21:25:09 by sabdulki          #+#    #+#             */
/*   Updated: 2024/06/21 22:19:54 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	append(int fd, char **argv, t_args *args)
{
	char	*input;
	char	*input_exp;

	input_exp = NULL;
	if (fd < 0)
		return (0);
	input = ft_strdup(argv[1]);
	if (!input)
		return (printf("malloc_fail\n"), 1);
	if (is_expantion(input))
	{
		input_exp = add_expantion(input, args);
		free(input);
		if (!input_exp)
			return (write(fd, "\n", 1), 1);
		input = input_exp;
	}
	write(fd, input, ft_strlen(input));
	write(fd, "\n", 1);
	close (fd);
	return (0);
}