/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 15:39:45 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/12 14:26:29 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	status_code(t_signal_type flag, int new_status)
{
	// static int	status_prev;
	static int	status;

	if (flag == GET)
		return (status);
	else if (flag == SET)
		status = new_status;
	// else if (flag == SET_HISTORY)
	// 	status_prev = status;
	// else if (flag == GET_HISTORY)
	// 	return (status_prev);
	return (status);
}

// int	get_status()
// {
// 	return (status);
// }

// int	set_status(int new_status)
// {
// 	status = new_status;
// 	return (status);
// }

void	handle_sigint(int sig)
{
	int status;
	if (sig == SIGINT)
	{
		// rl_catch_signals = 0;
		status = status_code(GET, -1);
		// fprintf(stderr, "status from get: '%d'\n", status);
		if (status == IN_CMD)
		{
			status_code(SET, STOP_CMD);
			// fprintf(stderr, "here!\n");
			g_exit_status = 130;
			write(STDERR_FILENO, "\n", 1);
			write_new_promt();
			return ;
		}
		if (status == IN_HEREDOC)
		{
			status_code(SET, STOP_HEREDOC);
			g_exit_status = 1; //or 130
			write(STDERR_FILENO, "\n", 1);
			write_new_promt();
			return ;
		}
		// rl_replace_line("", 0);
		write(STDERR_FILENO, "\n", 1);
		write_new_promt();
		return ;
	}
}
