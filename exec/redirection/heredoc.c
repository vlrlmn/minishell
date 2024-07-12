/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 14:17:50 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/12 18:04:54 by sabdulki         ###   ########.fr       */
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

/* if ctrl + C -> show new promt (minishell$) and exit with status 1
if ctrl + D exit with status 0 */

int	heredoc(t_cmd_info *cmd, char *limiter, t_args *args)
{
	int		fd;
	char	*input;
	char	*input_exp;

	input_exp = NULL;
	fd = get_file_fd(cmd, HEREDOC);
	if (fd == -1)
		return (-1);
	// status_code(SET, IN_HEREDOC);
	// fprintf(stderr, "limiter: '%s', its len:  %zu\n", limiter, ft_strlen(limiter));
	while (1)
	{
		status_code(SET, IN_HEREDOC);
		fprintf(stderr, "> ");
		input = get_next_line(STDIN_FILENO);
		if (!input || (input[0] == '\n' && !input[1])) // if ctrl + d. TODO ahnde ctrl+c
		{
			close(fd);
			if (input)
				free(input);
			// fprintf(stderr, "\nwarning: here-document delimited by end-of-file (wanted `%s')\n", limiter);
			return (-1);
		}
		input[ft_strlen(input) - 1] = '\0'; //remove '/n'
		// fprintf(stderr, "input: %s, its len:  %zu\n", input, ft_strlen(input));
		if ((ft_strncmp(limiter, input, ft_strlen(limiter)) == 0) && (ft_strlen(limiter) == (ft_strlen(input))))
		{
			free(input);
			break ;
		}
		if (is_expantion(input))
		{
			input_exp = add_expantion(input, args);
			if (!input_exp)
			{
				free(input);
				return (write(fd, "\n", 1), -1);
			}
			free(input);
			input = input_exp;
		}
		// fprintf(stderr, "input: '%s'\n", input);
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
		free(input);
	}
	close(fd);
	// fprintf(stderr, "heredoc completed\n");
	status_code(SET, IN_CMD);
	return (fd);
}

int	call_heredocs(char **arr, t_cmd_info *new_cmd, char **limiter_arr, t_args *args)
{
	int	i;
	int	size;
	char *limiter;
	
	i = -1;
	size = 0;
	while (arr[size])
		size++;
	size -= 1;
	while (i < size)
	{
		limiter = limiter_arr[size];
		new_cmd->file_read = arr[size];
		new_cmd->fd_read = heredoc(new_cmd, limiter, args);
		if (new_cmd->fd_read == -1)
			return (1);
		if (size != 0)
		{
			unlink(arr[size]);
			free(arr[size]);
		}
		size--;
	}
	free(arr);
	free(limiter_arr);
	new_cmd->fd_read = get_file_fd(new_cmd, HEREDOC);
	if (new_cmd->fd_read == -1)
		return (1);
	return (0);
}
