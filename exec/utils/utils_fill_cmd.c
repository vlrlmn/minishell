/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_fill_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 14:27:19 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/08 17:19:16 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	copy_eargv(t_cmd_info *new_cmd, t_cmd *cmd)
{
	t_execcmd	*ecmd;
	int			i;

	i = -1;
	ft_memset(new_cmd->eargv, 0, sizeof(new_cmd->eargv));
	if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		while (++i < MAXARGS && ecmd->eargv[i])
			new_cmd->eargv[i] = ecmd->eargv[i];
	}
	new_cmd->eargv[i] = NULL;
}

void	copy_argv(t_cmd_info *new_cmd, t_cmd *cmd)
{
	t_execcmd	*ecmd;
	int			i;

	i = -1;
	ft_memset(new_cmd->argv, 0, sizeof(new_cmd->argv));
	if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		while (++i < MAXARGS && ecmd->argv[i])
			new_cmd->argv[i] = ecmd->argv[i];
	}
	new_cmd->argv[i] = NULL;
}

int connection_content(t_cmd_info *new_cmd)
{
	new_cmd->connection = malloc(sizeof(int) * 2);
	if (!new_cmd->connection)
		return (1);
	new_cmd->connection[0] = 0; //default
	new_cmd->connection[1] = 1; //default
	return (0);
}

/* This function ensures 
that all non-empty arguments in ecmd->argv and ecmd->eargv are moved 
to the front of their respective arrays, 
eliminating any gaps caused by empty strings 
and correctly terminating the arrays with NULL */
void check_arguments(t_cmd_info *ecmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (ecmd->argv[i])
	{
		if (ecmd->argv[i][0] != '\0')
		{
			if (i != j)
			{
				ecmd->argv[j] = ecmd->argv[i];
				ecmd->eargv[j] = ecmd->eargv[i];
			}
			j++;
		}
		i++;
	}
	ecmd->argv[j] = NULL;
	ecmd->eargv[j] = NULL;
}

int check_file_access(const char *file_path, int mode) {
	/* if redir_type heredoc or redirin, check for existance. Dpends on the redir type check 
	F_OK: Check for existence of the file.
	R_OK: Check for read permission.
	W_OK: Check for write permission.
	*/
	// printf("mode: %d\n", mode);
    if (access(file_path, mode) == 0) {
        return (printf("File '%s' is accessible with the specified mode.\n", file_path), 0);
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
    return (1);
}
