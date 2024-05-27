/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aajaanan <aajaanan@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 08:25:55 by aajaanan          #+#    #+#             */
/*   Updated: 2023/09/02 14:33:36 by aajaanan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/signals.h"

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		ft_printf_fd(STDERR_FILENO, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (signum == SIGQUIT)
	{
	}
}

void	signal_handler_herdoc(int signum)
{
	int	fd;
	int	child_pid;

	if (signum == SIGINT)
	{
		ft_printf("\n");
		fd = open("/tmp/child_pid.tmp", O_RDONLY);
		if (fd < 0)
		{
			perror("open");
			exit(1);
		}
		if (read(fd, &child_pid, sizeof(int)) < 0)
		{
			perror("read");
			exit(1);
		}
		close(fd);
		kill(child_pid, SIGKILL);
	}
	else if (signum == SIGQUIT)
		ft_printf("\b\b  \b\b");
}

void	signal_handler_input(int signum)
{
	if (signum == SIGQUIT)
		ft_printf_fd(STDERR_FILENO, "Quit: %d\n", SIGQUIT);
	else if (signum == SIGINT)
		ft_printf_fd(STDERR_FILENO, "\n");
}

void	setup_signals(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	set_signal_handler(t_cmd *tree)
{
	if (tree && tree->type == REDIR)
	{
		signal(SIGINT, signal_handler_herdoc);
		signal(SIGQUIT, signal_handler_herdoc);
	}
	else
	{
		signal(SIGINT, signal_handler_input);
		signal(SIGQUIT, signal_handler_input);
	}
}
