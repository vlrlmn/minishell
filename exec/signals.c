/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 15:39:45 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/09 11:59:20 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int status;
// static int interrupted;
int interrupted;

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
		if (get_status() == 0) { // Shell is waiting for input
            printf("\n"); // Print a newline to move to the next line
            rl_on_new_line(); // Prepare readline to start a new line
            // rl_replace_line("", 0); // Clear the current input line
            rl_redisplay(); // Redisplay the prompt
            interrupted = 1; // Set the interruption flag
        } else { // Shell is executing a command
            g_exit_status = 130; // Set the exit status to 130
            interrupted = 0; // Reset the interruption flag
        }
		// rl_replace_line("", 0);
		// write(STDERR_FILENO, "\n", 1);
		// write_new_promt();
	}
}
