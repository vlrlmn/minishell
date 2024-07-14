/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 14:17:50 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/13 19:18:14 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
	free(num_str);
	if (!filename)
		return (printf("strjoin failed\n"), NULL);
	return (filename);
}

int	heredoc(t_cmd_info *cmd, char *limiter, t_args *args)
{
	int		fd;
	char	*input;

	fd = get_file_fd(cmd, HEREDOC);
	if (fd == -1)
		return (-1);
	while (1)
	{
		status_code(SET, IN_HEREDOC);
		fprintf(stderr, "> ");
		input = get_next_line(STDIN_FILENO);
		if (check_input(input, fd))
			return (-2);
		if (is_limiter(input, limiter))
			return (free(input), close(fd), (fd));
		input = do_expantion(input, cmd, args);
		if (!input)
			return (close(fd), -1);
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
		free(input);
	}
	close(fd);
	return (fd);
}

void	null_input_exit(char **arr, t_cmd_info *new_cmd)
{
	unlink(new_cmd->file_read);
	free_heredoc_arr(arr, 'n');
	new_cmd->file_read = NULL;
	close(new_cmd->fd_read);
}

int	call_heredocs(char **arr, t_cmd_info *new_cmd, t_args *args)
{
	int		size;
	char	*limiter;

	size = count_arr_elem(arr);
	while (-1 < size)
	{
		limiter = arr[size];
		size--;
		new_cmd->file_read = arr[size];
		new_cmd->fd_read = heredoc(new_cmd, limiter, args);
		if (new_cmd->fd_read == -2)
			return (null_input_exit(arr, new_cmd), 1);
		if (size != 0 && size % 2 == 0)
			unlink(arr[size]);
		size--;
	}
	new_cmd->file_read = free_heredoc_arr(arr, 'l');
	new_cmd->fd_read = get_file_fd(new_cmd, HEREDOC);
	if (new_cmd->fd_read == -1)
		return (1);
	return (0);
}
