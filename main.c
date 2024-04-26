/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlomakin <vlomakin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:44:21 by vlomakin          #+#    #+#             */
/*   Updated: 2024/04/26 11:53:38 by vlomakin         ###   ########.fr       */
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

int	loop_result(t_args *args)
{
	int		exit_status;
	char	*input;

	exit_status = 0;
	while (1)
	{
		(void)args;
		input = readline("minishell$ ");
		if (input == NULL)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (*input)
			add_history(input);
		lexer(input);
	}
	return (exit_status);
}

void	set_environment(t_args *args, char **envp)
{
	int	len;
	int	i;

	i = 0;
	len = 0;
	while (envp[len])
		len++;
	args->envp = (char **)malloc((len + 2) * sizeof(char *));
	while (i < len)
	{
		args->envp[i] = ft_strdup(envp[i]);
		i++;
	}
}

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
	free_environment(&shell_context); // TO_DO
	return (exit_status);
}
