/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 15:39:45 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/12 18:15:54 by lomakinaval      ###   ########.fr       */
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

void	handle_sigint(int sig)
{
	int status;
	if (sig == SIGINT)
	{
		status = status_code(GET, -1);
		if (status == IN_CMD)
		{
			status_code(SET, STOP_CMD);
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
		write(STDERR_FILENO, "\n", 1);
		write_new_promt();
		return ;
	}
}
