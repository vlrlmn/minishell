/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 15:39:45 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/07 17:41:42 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int status;

int	get_status()
{
	return (status);
}

int	set_status(int new_status)
{
	status = new_status;
	return (status);
}

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		status = get_status();
		if (status == IN_CMD)
		{
			set_status(STOP_CMD);
			// fprintf(stderr, "here!\n");
			// rl_redisplay();
			g_exit_status = 130;
		}
		if (status == IN_HEREDOC)
		{
			set_status(STOP_HEREDOC);
			g_exit_status = 1; //or 130
		}
		rl_replace_line("", 0);
		write(STDERR_FILENO, "\n", 1);
		write_new_promt();
	}
}
