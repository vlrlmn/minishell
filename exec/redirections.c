/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:23:17 by sabdulki          #+#    #+#             */
/*   Updated: 2024/06/12 20:24:33 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*heredoc_get_tmp_file(void)
{
	static int	tmp_file_num;
	char		*num_str;
	char		*filename;

	tmp_file_num += 1;
	num_str = ft_itoa(tmp_file_num);
	if (!num_str)
		return (NULL);
	filename = ft_strjoin("/var/tmp/heredoc_", num_str);
	if (!filename)
		return (printf("strjoin failed\n"), NULL);
	free(num_str);
	return (filename);
}

int	heredoc(t_redir *rcmd)
{
	/* new_fd = open(rcmd->file, rcmd->mode, 0644);
		but i'll open not a rcmd->file, i'll create tmp file and write all info from heredeoc there */
	int		new_fd;
	char	*input;
	char	*limiter;
	char	*filename;
	
	limiter = rcmd->file;
	filename = heredoc_get_tmp_file();
	if (!filename)
		return (1);
	rcmd->file = filename;
	new_fd = open(rcmd->file, rcmd->mode, 0777); // 0644
	if (new_fd < 0)
	{
		printf("open %s failed\n", rcmd->file);
		exit(126);
	}
	if (new_fd != rcmd->fd) // Close the old file descriptor if they are different
		close(rcmd->fd); // close 0 or 1
	rcmd->fd = new_fd;
	close (new_fd);
	fprintf(stderr, "limiter: %s, its len:  %zu\n", limiter, ft_strlen(limiter));
	while (1)
	{
		fprintf(stderr, "> ");
		input = get_next_line(STDIN_FILENO);
		if (!input) // if ctrl + d. TODO ahnde ctrl+c
		{
			//close fds, free memory
			fprintf(stderr, "NULL input\n");
			return (1);
		}
		// fprintf(stderr, "input len: %zu\n", ft_strlen(input));
		if ((ft_strncmp(limiter, input, ft_strlen(limiter)) == 0) && (ft_strlen(limiter) == (ft_strlen(input) - 1)))
		{
			free(input);
			break ;
		}
		write(rcmd->fd, input, ft_strlen(input)); // write into newly created file fd
		free(input);
	}
	close(rcmd->fd);
	fprintf(stderr, "heredoc completed\n");
	return (0);
}

void	redir(t_redir *rcmd)
{
	int		new_fd;

	new_fd = open(rcmd->file, rcmd->mode, 0644);
	if (new_fd < 0) //Implicit File Descriptor Assignment
	{
		printf("open '%s' failed\n", rcmd->file);
		// exit(126);
		return ;
	}
	if (new_fd != rcmd->fd) // Close the old file descriptor if they are different
		close(rcmd->fd);
	rcmd->fd = new_fd;
}