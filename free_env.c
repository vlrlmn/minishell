/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlomakin <vlomakin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:44:06 by vlomakin          #+#    #+#             */
/*   Updated: 2024/04/22 12:57:10 by vlomakin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_environment(t_args *shell_context)
{
	int	i;

	i = 0;
	while (shell_context->envp[i])
	{
		free(shell_context->envp[i]);
		i++;
	}
	free(shell_context->envp);
}

void	free_line_tokens(char **line_tokens)
{
	int i;

	i = 0;
	while (line_tokens[i])
	{
		free(line_tokens[i]);
		i++;
	}
	free(line_tokens);
}