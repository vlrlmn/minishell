#include "minishell.h"

char *handle_special_symbols(char *s, char *es, int *ret)
{
    if(*s == '|')
        s++;
    else if(*s == '<')
    {
        s++;
        if (*s == '<') 
        {
            *ret = '-';
            s++;
        }
    }
    else if (*s == '>')
    {
        s++;
        if (*s == '>') 
        {
            *ret = '+';
            s++;
        }
    }
    else
        handle_words()
}

int gettoken(char **ps, char *es, char **q, char **eq)
{
    char *s;
    int ret;

    s = *ps;
    while(s < es && is_delimiter(*s))
        s++;
    if(q)
        *q = s;
    ret = *s;
    handle_special_symbols(&s, es, &ret);
    if(eq)
        *eq = s;
    while(s < es && is_delimiter(*s))
        s++;
    *ps = s;
    return(ret);
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
