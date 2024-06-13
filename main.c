/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:44:21 by vlomakin          #+#    #+#             */
/*   Updated: 2024/06/13 17:03:24 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int exit_status = 0;

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		write_new_promt();
	}
}

void	write_new_promt(void)
{
	// rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void PrintTree(t_cmd	*cmd)
{
	t_execcmd *exec;
	t_pipe *pipe;
	t_redir *redir;

	int i = 0;

	if (cmd->type == EXEC)
	{
		exec = (t_execcmd*) cmd;
		i=0;
		while (exec->argv[i])
		{
			
			// printf("Arg %d: %.*s\n", i, (int)(exec->eargv[i] - exec->argv[i]), exec->argv[i]);
			printf("EARGV %s\n", exec->argv[i]);
			printf("ARGV %s\n", exec->eargv[i]);
			i++;
		}
	}
	else if (cmd->type == PIPE)
	{
		pipe = (t_pipe*) cmd;
		PrintTree(pipe->left);
		PrintTree(pipe->right);
	}
	else if (cmd->type == REDIR)
	{
		redir = (t_redir*)cmd;
		printf("\nsubtype '%d'\n", redir->subtype);
		printf("redir mode '%d'\n", redir->mode);
		printf("redir file '%s'\n", redir->file);
		printf("redir efile '%s'\n", redir->efile);
		PrintTree(redir->cmd);
		
	}
	else
	{
		printf("Unknown Type\n");
	}
}

int ft_launch_minishell(t_args *args)
{
	t_cmd	*cmd;
	

    if (args->input == NULL)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		return 1;
	}
	if (!valid_input(args->input))
	{
		free_envp(args);
		exit(SYNTAX_ERR);
	}
	//if (fork1() == 0)
	//{
		cmd = parse(args);
		PrintTree(cmd);
		run_cmd(cmd, args);
	//}
	//wait (0);
	return (0);
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
			exit_status = 2;
		}
		add_history(args->input);
		cmd = parse(args);
		//it creates child proc ONCE for one input. For second input it'll create another child proc etc...
		pid_t pid = fork1();
		if (pid == 0)
		{
			run_cmd(cmd, args);
			exit(0);
		}
		else if (pid > 0)
		{
			waitpid(pid, NULL, 0);
		}
		else
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
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
	// if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
	// {
	// 	shell_context.input = argv[2];
	// 	exit_status = ft_launch_minishell(&shell_context);
	// 	return (exit_status);
	// }
	exit_status = loop_result(&shell_context);
	// rl_clear_history(); //idk why mac argue for it
	// rl_clear_history();
	clear_history();
	free_envp (&shell_context);
	return (exit_status);
}

/*
about signals
1) should ctrl+c clear all memory or returning promt is enough? should there be ^C after pressing this cmd?
2) if 'return' pressed, should it quit? errno = 2
3) ctrl D -> "exit" str in the promt, not on the new line
*/