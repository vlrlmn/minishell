#include "minishell.h"

int gettoken(char **ps, char *es, char **q, char **eq)
{
    char *s;
    int ret;

    s = *ps;
    while(s < es && is_delimiter(*s))
        s++;
    if(q)
        *q = s;
    if (s >= es)
        ret = 0;
    
}

int peek(char **ps, char **es, char *toks)
{
    char *s;

    s = *ps;
    while(s < es && is_delimiter(*s))
        s++;
    *ps = s;
    return(*s && ft_strchr(toks, *s));
}
