#include "minishell.h"

int valid_quotes(char *input)
{
    int double_q;
    int single_q;
    int i;

    double_q = 0;
    single_q = 0;
    i = -1;
    while(input[i++])
    {
        if(input[i] == '\'')
            single_q++;
        if (input[i] == '\"')
            double_q++;
    }
    if (single_q % 2 != 0 || double_q % 2 != 0)
        return(QUOTES_ERR);
    else if (single_q && double_q)
        return (MULTIPLE_QUOTES);
    else if (single_q && !double_q)
        return (SINGLE_Q);
    else if (!single_q && double_q)
        return(DOUBLE_Q);
    else
        return (NO_QUOTES);
}

int valid_parenthesis(char *input)
{
    int i;
    int count;
    
    i = 0;
    count = 0;
    while(input[i])
    {
        if (input[i] == '(')
            count++;
        else if (input[i] == ')')
        {
            if (count == 0)
                return(-1);
            count--;
        }
        i++;
    }
    if (count != 0)
        return(0);
    else
        return(1);
}

void add_spaces(char *work_line, char *input, int input_len)
{
    int i;
    int j;
    int double_q;
    int single_q;

    i = 0;
    j = 0;
    double_q = 0;
    single_q = 0;
    while(i < input_len)
    {
        if(input[i] == '\'' && !double_q)
            single_q = !single_q;
        if (input[i] == '\"' && !single_q)
            double_q = !double_q;
        if (!single_q && !double_q && (input[i] == '>' || input[i] == '<' || input[i] == '|'))
        {
            if(j > 0 && work_line[j - 1] != ' ')
                work_line[j++] = ' ';
            if (input[i] == '>' && input[i + 1] == '>')
                work_line[j++] = input[++i];
            work_line[j++] = ' ';
        }
        else
            work_line[j++] = input[i];
        i++;
    }
    work_line[j] = '\0';
}

void lexer(char *input)
{
    char **line_tokens;
    char *work_line;
    int len;

    len = ft_strlen(input);
    if (valid_quotes(input) == QUOTES_ERR || !valid_parenthesis(input))
        exit_with_syntax_err(SYNTAX_ERR);
    work_line = malloc(sizeof(char) * (len * 4)); 
    if (!work_line)
        exit_with_malloc_error(EX_UNAVAILABLE);
    add_spaces(work_line, input, len);
    line_tokens = ft_split(work_line, ' ');
    if (!line_tokens)
        exit_with_malloc_error(EX_UNAVAILABLE);
    free(line_tokens);
    free(work_line);
}