#include "minishell.h"

void free_structs() //TO_DO
{
}

void exit_with_syntax_err(int err_code)
{
    write(2, "Quotes err\n", 12);
    free_structs(); //TO_DO
    exit(err_code);
}

void exit_with_malloc_error(int err_code)
{
    write(2, "Malloc err\n", 12);
    free_structs(); //TO_DO
    exit(err_code);
}
