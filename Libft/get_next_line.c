/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:39:33 by vlomakin          #+#    #+#             */
/*   Updated: 2024/07/14 18:54:30 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <signal.h>

#define BUFFER_SIZE 1000

void	handle_sigint_gnl(int sig, t_signal_state *state)
{
	(void)sig;
	state->interrupted = 1;
}

int	setup_signal_handler(t_signal_state *state)
{
	struct sigaction	sa;

	sa.sa_handler = (void (*)(int))handle_sigint_gnl;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	(void)state;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		return (-1);
	}
	return (0);
}

char	*read_to_n(int fd, char *save_line, t_signal_state *state)
{
	int		bytes_read;
	char	*buf;

	if (!save_line)
		save_line = ft_calloc(1, 1);
	buf = ft_calloc(sizeof(char), BUFFER_SIZE + 1);
	bytes_read = 1;
	while (state->interrupted != 1 || bytes_read > 0)
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			state->interrupted = 0;
			return (free(buf), free(save_line), NULL);
		}
		else if (bytes_read == 0)
			break ;
		buf[bytes_read] = '\0';
		save_line = join_read(save_line, buf);
		if (ft_strchr(save_line, '\n'))
			break ;
	}
	free(buf);
	return (save_line);
}

char	*get_next_line(int fd)
{
	static char		*save_line;
	char			*result;
	t_signal_state	state;

	state.interrupted = 0;
	if (setup_signal_handler(&state) == -1)
		return (NULL);
	if (fd < 0 || BUFFER_SIZE <= 0 || (read(fd, 0, 0) < 0))
		return (NULL);
	save_line = read_to_n(fd, save_line, &state);
	if (!save_line)
		return (NULL);
	result = final_line(save_line);
	save_line = remainder_chars(save_line);
	return (result);
}
