#include "minishell.h"

void handle_sigint(int sig) 
{
    if (sig == SIGINT)
    {
        write(STDERR_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
    }

}

int loop_result(t_args *args)
{
    int exit_status;
    char *input;
    while(1)
    {
        input = readline("minishell$ ");
        if (input == NULL)
        {
            write(STDOUT_FILENO, "exit\n", 5);
            break ;
        }
        if (input)
            add_history(input);
        lexer(args, input);
    }
    return (exit_status);
}

void set_environment(t_args *args, char **envp)
{
    int len;

    len = 0;
    while(envp[len])
        len++;
    args->envp = (char **)malloc((len + 2) * sizeof(char *));
    //write function which will copy **envp to args->envp

}

int main(int argc, char **argv, char **envp)
{
    t_args args;
    int exit_status;

    set_environment(&args, envp);
    signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
    exit_status = loop_result(&args);
    rl_clear_history();
    return(exit_status);
}