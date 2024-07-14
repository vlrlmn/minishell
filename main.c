/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlomakin <vlomakin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:44:21 by vlomakin          #+#    #+#             */
/*   Updated: 2024/07/14 18:59:07 by vlomakin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_sig_exit_status = 0;

void	write_new_promt(void)
{
	// rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
}

void	exec(t_cmd *cmd, t_args *args, int *last_status)
{
	t_cmd_info	*cmd_list;
	int			**pipe_arr;

	pipe_arr = NULL;
	cmd_list = create_cmdlist(cmd, args, last_status);
	if (status_code(GET, -1) == CTRL_D)
		*last_status = 0;
	if (!cmd_list)
		return (free_all(cmd_list, pipe_arr));
	pipe_arr = connections(cmd_list);
	*last_status = run_cmds(cmd_list, pipe_arr, args);
	if (!cmd_list->argv[0] || cmd_list->argv[0][0] == '\0')
		return (free_all(cmd_list, pipe_arr));
	if (list_size(cmd_list) == 1 && is_buildin(cmd_list->argv[0]))
		return (free_all(cmd_list, pipe_arr));
	*last_status = wait_cmds(cmd_list);
	free_all(cmd_list, pipe_arr);
	return ;
}

void	loop_result(t_args *args, int *exit_status)
{
	t_cmd	*cmd;

	while (1)
	{
		args->input = readline("minishell$ ");
		if (args->input == NULL)
			break ;
		if (ft_strlen(args->input) > 1 && args->input[0] != '\n')
			add_history(args->input);
		if (!valid_input(args->input, exit_status))
			continue ;
		cmd = parse(args, exit_status);
		exec(cmd, args, exit_status);
	}
	return ;
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

	exit_status = 0;
	(void)argc;
	(void)argv;
	set_environment(&shell_context, envp);
	signal(SIGQUIT, handle_sigquit);
	signal(SIGINT, handle_sigint);
	loop_result(&shell_context, &exit_status);
	clear_history();
	free_envp(&shell_context);
	return (exit_status);
}
