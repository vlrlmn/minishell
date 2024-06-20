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
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void check_file_access(const char *file_path, int mode) {
    if (access(file_path, mode) == 0) {
        printf("File '%s' is accessible with the specified mode.\n", file_path);
    } else {
        // access() failed, check errno to determine the error
        switch (errno) {
            case EACCES:
                printf("Error: Permission denied to access '%s'.\n", file_path);
                break;
            case EROFS:
                printf("Error: Read-only file system, cannot write to '%s'.\n", file_path);
                break;
            case ENOENT:
                printf("Error: File '%s' does not exist.\n", file_path);
                break;
            case ENOTDIR:
                printf("Error: A component of the path '%s' is not a directory.\n", file_path);
                break;
            case ENAMETOOLONG:
                printf("Error: The path '%s' is too long.\n", file_path);
                break;
            case ELOOP:
                printf("Error: Too many symbolic links encountered while accessing '%s'.\n", file_path);
                break;
            case EFAULT:
                printf("Error: Bad address for file '%s'.\n", file_path);
                break;
            case EINVAL:
                printf("Error: Invalid mode specified for accessing '%s'.\n", file_path);
                break;
            case ENOMEM:
                printf("Error: Insufficient kernel memory to access '%s'.\n", file_path);
                break;
            case ETXTBSY:
                printf("Error: Text file busy, cannot write to '%s'.\n", file_path);
                break;
            default:
                printf("Error: Failed to access '%s' with error code %d (%s).\n", file_path, errno, strerror(errno));
                break;
        }
    }
}

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

int	old_heredoc(t_redir *rcmd)
{
	// if heredoc gets the expander, it should return its value!
	int		new_fd;
	// char	*input;
	char	*limiter;
	char	*filename;
	
	limiter = rcmd->file;
	filename = heredoc_get_tmp_file();
	if (!filename)
		return (1);
	rcmd->file = filename;
	check_file_access(rcmd->file, R_OK);
	//
	new_fd = open(rcmd->file, rcmd->mode, 0777); // 0644
	if (new_fd < 0)
	{
		printf("open %s failed in heredoc\n", rcmd->file);
		exit(126);
	}
	rcmd->fd = new_fd;
	// close (new_fd);
	fprintf(stderr, "limiter: %s, its len:  %zu\n", limiter, ft_strlen(limiter));
	// while (1)
	// {
	// 	fprintf(stderr, "> ");
	// 	input = get_next_line(STDIN_FILENO);
	// 	if (!input) // if ctrl + d. TODO ahnde ctrl+c
	// 	{
	// 		//close fds, free memory
	// 		close(rcmd->fd);
	// 		fprintf(stderr, "NULL input\n");
	// 		return (1);
	// 	}
	// 	// fprintf(stderr, "input len: %zu\n", ft_strlen(input));
	// 	if ((ft_strncmp(limiter, input, ft_strlen(limiter)) == 0) && (ft_strlen(limiter) == (ft_strlen(input) - 1)))
	// 	{
	// 		free(input);
	// 		break ;
	// 	}
	// 	write(rcmd->fd, input, ft_strlen(input)); // write into newly created file fd
	// 	free(input);
	// }
	close(rcmd->fd);
	fprintf(stderr, "heredoc completed\n");
	return (0);
}
/* When you use >>, 
Bash opens the specified file in append mode and writes the output of the command to it. 
If the file does not exist, it will be created. */
// int	append()
// {

// }

void	redir(t_redir *rcmd)
{
	int		new_fd;

	new_fd = open(rcmd->file, rcmd->mode, 0644);
	if (new_fd < 0) //Implicit File Descriptor Assignment
	{
		printf("open '%s' failed in redir\n", rcmd->file);
		exit(126);
	}
	if (new_fd != rcmd->fd) // Close the old file descriptor if they are different
		close(rcmd->fd);
	rcmd->fd = new_fd;
}

/* if ctrl + C -> show new promt (minishell$) and exit with status 1
if ctrl + D exit with status 0 */
int	heredoc(int fd, char *file, char *limiter, int mode, t_args *args)
{
	// if heredoc gets the expander, it should return its value!
	char	*input;
  
	check_file_access(file, R_OK);
	fd = get_file_fd(fd, file, mode);
	fprintf(stderr, "limiter: %s, its len:  %zu\n", limiter, ft_strlen(limiter));
	while (1)
	{
		fprintf(stderr, "> ");
		input = get_next_line(STDIN_FILENO);
		if (!input) // if ctrl + d. TODO ahnde ctrl+c
		{
			//close fds, free memory
			close(fd);
			fprintf(stderr, "NULL input\n");
			return (1);
		}
		// fprintf(stderr, "input len: %zu\n", ft_strlen(input));
		if ((ft_strncmp(limiter, input, ft_strlen(limiter)) == 0) && (ft_strlen(limiter) == (ft_strlen(input) - 1)))
		{
			free(input);
			break ;
		}
		input = add_expantion(input, args);
		write(fd, input, ft_strlen(input)); // write into newly created file fd
		free(input);
	}
	close(fd);
	fprintf(stderr, "heredoc completed\n");
	return (fd);
}

char	*add_expantion(char *input, t_args *args)
{
    size_t	i;
	char	*var_value;
	char	*env_var;
	char	*result;

	i = 0;
	while (input[i] && input[i] != '$')
		i++;
	if (i == (ft_strlen(input))) //if there are no '$'
	{
		fprintf(stderr, "no expantions!\n");
		return (input);
	}
	env_var = get_str_after_sign(input, '$'); //free
	if (!env_var)
		return (NULL);
	fprintf(stderr, "env_var: %s\n", env_var);
	env_var[ft_strlen(env_var) - 1] = '\0';
    var_value = find_env_var(args->envp, env_var);
	fprintf(stderr, "var_value: %s\n", var_value);
    if (!var_value)
        return (input); // or "\0\n" ??
    printf("exp: %s\n", var_value);
	// return input, insted of var name it's meaning
	result = malloc(sizeof(char) * (i-1) + ft_strlen(var_value) + 2);
	if (!result)
		return (NULL);
	
    return (result);
}