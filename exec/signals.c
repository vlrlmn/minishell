/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 15:39:45 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/14 20:02:59 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	status_code(t_signal_type flag, int new_status)
{
	static int	status;

	if (flag == GET)
		return (status);
	else if (flag == SET)
		status = new_status;
	return (status);
}

void	handle_sigquit(int sig)
{
	if (sig == SIGQUIT)
	{
		status_code(SET, CTRL_D);
		g_sig_exit_status = 0;
	}
	return ;
}

void	write_sig_prompt(void)
{
	write(STDERR_FILENO, "\n", 1);
	write_new_promt();
}

void	handle_sigint(int sig)
{
	int	status;

	if (sig == SIGINT)
	{
		status = status_code(GET, -1);
		if (status == IN_CMD)
		{
			status_code(SET, STOP_CMD);
			g_sig_exit_status = 130;
			write(STDERR_FILENO, "\n", 1);
			write_new_promt();
			return ;
		}
		if (status == IN_HEREDOC)
		{
			status_code(SET, STOP_HEREDOC);
			g_sig_exit_status = 1;
			write(STDERR_FILENO, "\n", 1);
			write_new_promt();
			return ;
		}
		write_sig_prompt();
		g_sig_exit_status = 0;
		return ;
	}
}
