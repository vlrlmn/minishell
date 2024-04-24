#include "minishell.h"

int handle_quotes(char **input, int *in_quote, char **start, t_token **head)
{
    if (**input == 34 || **input == 39)
    {
        if(!*in_quote)
        {
            *in_quote = **input;
            *start = *input + 1;
        }
        else if (*in_quote == **input)
        {
            add_token(head, *start, *input, QUOTE);
            *in_quote = 0;
            *start = *input + 1;
        }
        return (1);
    }
    return (0);
}

void tokenize_input(char *input)
{
    int in_quote;
    t_token *head;
    char *start;

    start = input;
    head = NULL;
    in_quote = 0;
    while(*input)
    {
        if (handle_quotes(&input, &in_quote, &start, &head))
        {
            input++;
            continue;
        }
        if (!in_quote && (*input == ' ' || *input == '\t'))
        {
            if (start < input)
                add_token(&head, start, input, WORD);
            start = input + 1;
        }
        input++;
    }
    if (start < input)
        add_token(&head, start, input, WORD);
}
