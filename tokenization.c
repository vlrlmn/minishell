#include "minishell.h"

void add_token(t_token **head, char *start, char *end, t_type type)
{
    t_token *new;
    t_token *cur;

    new = malloc(sizeof(t_token));
    cur =  NULL;
    if (!new)
        return ;
    new->value = strndup(start, end - start); //TO_DO
    if (!new->value)
    {
        free(new);
        return ;
    }
    new->type = type;
    new->next = NULL;
    if (!*head)
        *head = new;
    else
    {
        cur = *head;
        while(cur->next)
            cur = cur->next;
        cur->next = new;
    }
}

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

const char *token_type_string(t_type type) {
    switch (type) {
        case WORD: return "WORD";
        case QUOTE: return "QUOTE";
        case PIPE: return "PIPE";
        case REDIR_IN: return "REDIR_IN";
        case REDIR_OUT: return "REDIR_OUT";
        default: return "UNKNOWN";
    }
}

void print_tokens(const t_token *tokens) {
    const t_token *current = tokens;
    printf("Tokens:\n");
    while (current != NULL) {
        printf("Type: %s, Value: %s\n", token_type_string(current->type), current->value);
        current = current->next;
    }
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
    print_tokens(head);
}
