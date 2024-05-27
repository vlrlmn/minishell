/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:44:21 by vlomakin          #+#    #+#             */
/*   Updated: 2024/05/27 19:57:29 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

/*This is where we have instant loop happening. Inside the loop
we reading the line, adding it in history and call lexer, beginning
of args->input parsing*/
int	loop_result(t_args *args)
{
	t_cmd	*cmd;

	while (1)
	{
		args->input = readline("minishell$ ");
		if (args->input == NULL)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (!valid_input(args->input))
		{
			free_envp(args);
			exit(SYNTAX_ERR);
		}
		if (fork1() == 0)
		{
			add_history(args->input);
			cmd = parse(args);
			cmd->params = args;
			run_cmd(cmd);
		}
		wait(0);
	}
	return (0);
}

/*We need to create new environment argument and copy envp from main
arguments because this is safe way to work with environment*/
void	set_environment(t_args *args, char **envp)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
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
}

/*Here we launch our program, set environment, handle signals.
For saving history and mowing with arrows through
previously written commands we use	rl_clear_history(void);
In the end we clear everything and free memory*/
int	main(int argc, char **argv, char **envp)
{
	t_args	shell_context;
	int		exit_status;

	(void)argc;
	(void)argv;
	set_environment(&shell_context, envp);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	exit_status = loop_result(&shell_context);
	rl_clear_history();
	free_envp (&shell_context);
	return (exit_status);
}
//HELLO//