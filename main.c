/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:44:21 by vlomakin          #+#    #+#             */
/*   Updated: 2024/07/12 19:31:34 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_status = 0;

void	write_new_promt(void)
{
	rl_on_new_line();
	rl_redisplay();
}

int	exec(t_cmd	*cmd, t_args *args)
{
	t_cmd_info	*cmd_list;
	int			**pipe_arr;
	int			exit_status;

	pipe_arr = NULL;
	cmd_list = create_cmdlist(cmd, args);
	// printf("status: %d\n", status_code(GET, -1));
	if (status_code(GET, -1) == CTRL_D)
		g_exit_status = 0;
	if (!cmd_list)// or return g_exit_status, which i need to define in case of failure inside the create_cmdlist()
		return (free_all(cmd_list, pipe_arr), g_exit_status); 
	pipe_arr = connections(cmd_list);
	// PrintList(cmd_list);
	// printPipeArr(pipe_arr);
	exit_status = run_cmds(cmd_list, pipe_arr, args);
	if (!cmd_list->argv[0] || cmd_list->argv[0][0] == '\0')
		return (free_all(cmd_list, pipe_arr), exit_status);
	if (list_size(cmd_list) == 1 && is_buildin(cmd_list->argv[0]))
		return (free_all(cmd_list, pipe_arr), exit_status);
	exit_status = wait_cmds(cmd_list);
	free_all(cmd_list, pipe_arr);
	return (exit_status);
}

int	loop_result(t_args *args)
{
	t_cmd	*cmd;

	while (1)
	{
		args->input = readline("minishell$ ");
		if (args->input == NULL)
			break ;
		add_history(args->input);
		if (!valid_input(args->input))
			continue ;
		cmd = parse(args);
		g_exit_status = exec(cmd, args);
	}
	return (g_exit_status);
}

void	set_environment(t_args *args, char **envp)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	args->envp = NULL;
	while (envp[len])
		len++;
	args->envp = (char **)malloc((len + 1) * sizeof(char *));
	if (!args->envp)
		exit_with_malloc_error(MALLOC_ERROR);
	while (i < len)
	{
		args->envp[i] = ft_strdup(envp[i]);
		if (!args->envp[i])
			panic_and_free_env(args, i);
		i++;
	}
	args->envp[len] = NULL;
	export_cmd("OLDPWD=", args);
}

int	main(int argc, char **argv, char **envp)
{
	t_args	shell_context;
	int		exit_status;

	(void)argc;
	(void)argv;
	set_environment(&shell_context, envp);
	signal(SIGQUIT, handle_sigquit);
	signal(SIGINT, handle_sigint);
	exit_status = loop_result(&shell_context);
	clear_history();
	// free_envp (&shell_context);
	return (exit_status);
}
