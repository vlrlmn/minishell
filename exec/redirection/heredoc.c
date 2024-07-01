/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 14:17:50 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/01 14:28:40 by sabdulki         ###   ########.fr       */
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
int	heredoc(int fd, char *file, char *limiter, int mode, t_args *args)
{
	// if heredoc gets the expander, it should return its value!
	char	*input;
	char	*input_exp;

	input_exp = NULL;
	// if (check_file_access(file, R_OK))
	// 	return (-1);
	fd = get_file_fd(fd, file, mode);
	// fprintf(stderr, "limiter: %s, its len:  %zu\n", limiter, ft_strlen(limiter));
	while (1)
	{
		fprintf(stderr, "> ");
		input = get_next_line(STDIN_FILENO);
		// if (signal(SIGINT, handle_sigint))
		// {
		// 	free(input);
		// 	close(fd);
		// 	return (130);
		// }
		if (!input) // if ctrl + d. TODO ahnde ctrl+c
		{
			close(fd);
			return (fprintf(stderr, "NULL input\n"), -1);
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
			free(input);
			if (!input_exp)
				return (write(fd, "\n", 1), -1);
			input = input_exp;
		}
		write(fd, input, ft_strlen(input)); // write into newly created file fd
		write(fd, "\n", 1);
		// free(input);
	}
	close(fd);
	// fprintf(stderr, "heredoc completed\n");
	return (fd);
}
