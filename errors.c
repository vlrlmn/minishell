#include "minishell.h"

void free_structs()
{
}

void exit_with_syntax_err(int err_code)
{
    free_structs(); // write which structures to free
    exit(err_code);
}

void exit_with_malloc_error(int err_code)
{
    free_structs(); // write which structures to free
    exit(err_code);
}
