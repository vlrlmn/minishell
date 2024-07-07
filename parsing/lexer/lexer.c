/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 17:36:36 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/07 15:45:44 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void free_list(t_lexems *list)
{
    t_lexem_node *current_node;
    t_lexem_node *next_node;

    if (list == NULL)
        return;

    current_node = list->head;
    while (current_node != NULL)
    {
        next_node = current_node->next;
        free(current_node->data);
        free(current_node);
        current_node = next_node;
    }
}

char *clean_line(char *line, t_lexems *list, t_args *args)
{
    char *res;
    int i;

    i = 0;
    // if (!line)
    //     return (NULL);
    while(line[i])
    {
        if (line[i] == '\'')
            parse_quote(line, &i, list);
        else if (line[i] == '\"')
            parse_double_quote(&i, line, list, args);
        else if (line[i] == '$')
            parse_expander(&i, list, line, args);
        else
        {
            add_char_node(list, line[i]);
            i++;
        }
    }
    if (i == 2 && line[0] == '\'' && line[i - 1] == '\'')
    {
       add_str_node(list, "\'\'");
    }
    res = list_to_string(list);
    //free_list(list);
    return (res);
}

char *clean_cmd(char *line, t_args *args)
{
    t_lexems  args_list;
    char *val;

    args_list.head = NULL;
    args_list.tail = NULL;
    if (ft_strchr(line, '~') && !ft_isalnum((*line + 1)))
    {
        val = ft_strdup(get_env("HOME", args->envp));
        return (val);
    }
    val = clean_line(line, &args_list, args);
    return(val);
}

void lexical_analysis(t_cmd *cmd, t_args *args)
{
    t_execcmd *exec;
    t_redir *rcmd;
    t_pipe *pcmd;
    int i;
   // char* tmp;

    i = 0;
    if (cmd->type == EXEC)
    {
        exec = (t_execcmd *)cmd;
        while (exec->argv[i])
        {
            if (has_parse_symbol(exec->argv[i]))
                exec->argv[i] = clean_cmd(exec->argv[i], args);
            else
            {
                //tmp = exec->argv[i];
                exec->argv[i] = ft_strdup(exec->argv[i]);
               // free(tmp);
            }
            i++;
        }
    }
    else if (cmd->type == REDIR)
    {
        rcmd = (t_redir *)cmd;
        if (rcmd->type == '-' && has_parse_symbol(rcmd->file)) // CHECK IF RCMD TYPE IS SAVED
            rcmd->file = clean_cmd(rcmd->file, args);
        lexical_analysis(rcmd->cmd, args);
    }
    else if (cmd->type == PIPE)
    {
        pcmd = (t_pipe *)cmd;
        lexical_analysis(pcmd->left, args);
        lexical_analysis(pcmd->right, args);
    }
}
