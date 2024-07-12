/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_fill_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 14:27:19 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/12 14:53:59 by sabdulki         ###   ########.fr       */
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

int	connection_content(t_cmd_info *new_cmd)
{
	new_cmd->connection = malloc(sizeof(int) * 2);
	if (!new_cmd->connection)
		return (1);
	new_cmd->connection[0] = 0;
	new_cmd->connection[1] = 1;
	return (0);
}

/* This function ensures
that all non-empty arguments in ecmd->argv and ecmd->eargv are moved
to the front of their respective arrays,
eliminating any gaps caused by empty strings
and correctly terminating the arrays with NULL */
void	check_arguments(t_cmd_info *ecmd)
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

int	check_file_access(const char *file_path, int redir_type)
{
	(void)redir_type;
	int	res;

	res = 0;
	if (redir_type == REDIRIN || redir_type == HEREDOC)
	{
		res = access(file_path, F_OK);
		printf("res F_OK?: %d\n", res);
        if (res == 0)
            res = access(file_path, R_OK);
	}
	if (redir_type == REDIROUT || redir_type == APPEND)
	{
		res = access(file_path, F_OK);
		printf("res F_OK?: %d\n", res);
        if (res == 0)
            res = access(file_path, W_OK);
	}
	printf("final res: %d\n", res);
	return (res);
}
