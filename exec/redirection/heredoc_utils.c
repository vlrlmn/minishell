/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 18:22:46 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/13 18:24:02 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int is_limiter(char *limiter, char *input)
{
	limiter[ft_strlen(limiter) - 1] = '\0';
	if (ft_strncmp(limiter, input, ft_strlen(limiter)) == 0 \
		&& ft_strlen(limiter) == (ft_strlen(input)))
	{
		return (1);
	}
	return (0);
}

char *do_expantion(char *input, t_cmd_info *cmd, t_args *args)
{
	char	*input_exp;

	input_exp = NULL;
	if (is_expantion(input))
	{
		input_exp = add_expantion(input, args);
		if (!input_exp)
			return (free(input), write(cmd->fd_read, "\n", 1), NULL);
		free(input);
		input = input_exp;
	}
	return (input);
}

int	check_input(char *input, int fd)
{
	if (!input || (input[0] == '\n' && !input[1])) // if ctrl + d. TODO ahnde ctrl+c
	{
		status_code(SET, CTRL_D);
		close(fd);
		if (input)
			free(input);
		return (1);
	}
	return (0);
}

void	free_heredoc_arr(char **arr)
{
	int i = 0;
	while(arr[i])
	{
		free(arr[i]);
		i += 2;
	}
	free(arr);
}

int	count_arr_elem(char **arr)
{
	int	size;

	size = 0;
	while (arr[size])
		size++;
	size -= 1;
	return (size);
}
