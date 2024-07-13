int gettoken(char **ps, char *es, char **q, char **eq)
{
    int token;

    while (*ps < es && (**ps == ' ' || **ps == '\t' || **ps == '\n'))
        (*ps)++;
    if (*ps >= es)
        return 0;
    token = **ps;
    if (**ps == '|')
    {
        (*ps)++;
        *eq = *ps;
        return token;
    }
    else if (**ps == '<' || **ps == '>')
    {
        (*ps)++;
        if (**ps == '<')
        {
            (*ps)++;
            while (is_delimiter(**ps))
                (*ps)++;
            *q = *ps;
            while (!is_delimiter(**ps) && !ft_strchr("|<>", **ps))
                (*ps)++;
            *eq = *ps;
            token = '+';
            return (token);
        }
        else if (**ps == '>')
        {
            (*ps)++;
            while (is_delimiter(**ps))
                (*ps)++;
            *q = *ps;
            while (!is_delimiter(**ps) && !ft_strchr("|<>", **ps))
                (*ps)++;
            *eq = *ps;
            token = '-';
            return (token);
        }
        while (is_delimiter(**ps))
            (*ps)++;
        *q = *ps;
        while (!is_delimiter(**ps) && !ft_strchr("|<>", **ps))
            (*ps)++;
        *eq = *ps;
        return token;
    }
    *q = *ps;
    while (*ps < es && !is_delimiter(**ps) && !ft_strchr("<>|", **ps))
    {
        if (**ps == '"')
        {
            (*ps)++;
            while(*ps < es && **ps != '"')
                (*ps)++;
            if (*ps >= es)
                return (0);
            (*ps)++;
        }
        else if (**ps == '\'')
        {
            (*ps)++;
            while(*ps < es && **ps != '\'')
                (*ps)++;
            if (*ps >= es)
                return (0);
            (*ps)++;  
        }
        else if (*ps < es)
            (*ps)++;
    }
    *eq = *ps;
    return ('a');
}
