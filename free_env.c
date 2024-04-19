#include "minishell.h"

void free_environment(t_args	*shell_context)
{
    int i;

    i = 0;
    while(shell_context->envp[i])
    {
        free(shell_context->envp[i]);
        i++;
    }
    free(shell_context->envp);
}
