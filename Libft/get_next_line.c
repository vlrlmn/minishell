/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:39:33 by vlomakin          #+#    #+#             */
/*   Updated: 2024/07/13 20:15:37 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <signal.h>

#define BUFFER_SIZE 1000

volatile sig_atomic_t interrupted = 0;

void handle_sigint_gnl(int sig) {
	(void)sig;
    interrupted = 1;
}

int setup_signal_handler() {
    struct sigaction sa;
    sa.sa_handler = handle_sigint_gnl;
    sa.sa_flags = 0; // или SA_RESTART
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return -1;
    }
    return 0;
}

char	*read_to_n(int fd, char *save_line)
{
	int		bytes_read;
	char	*buf;

	if (!save_line)
		save_line = ft_calloc(1, 1);
	buf = ft_calloc(sizeof(char), BUFFER_SIZE + 1);
	bytes_read = 1;
	while (interrupted != 1 || bytes_read > 0)
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			interrupted = 0;
			return (free(buf), free(save_line), NULL);
		}
		else if (bytes_read == 0) // EOF condition
            break;
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
	static char	*save_line;
	char		*result;

	if (setup_signal_handler() == -1)
        return (NULL);
	if (fd < 0 || BUFFER_SIZE <= 0 || (read(fd, 0, 0) < 0))
		return (NULL);
	save_line = read_to_n(fd, save_line);
	if (!save_line)
		return (NULL);
	result = final_line(save_line);
	save_line = remainder_chars(save_line);
	return (result);
}

// #include <stdio.h>

// int	main(void)
// {
// 	int fd;

// 	fd = open("txt.txt", O_RDONLY);
// 	if (fd < 0)
// 		exit(-1);
//  	char *line = get_next_line(fd);

// 	while (line)
// 	{
// 		printf("%s", line);
// 		free(line);
//  		line = get_next_line(fd);
// 	}
// 	printf("%s", get_next_line(fd));
// 	close(fd);
// }