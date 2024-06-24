/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 17:36:36 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/06/24 16:05:39 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *clean_line(char *line, t_lexems *list, t_args *args)
{
    char *res;
    int i;

    i = 0;
    while(line[i])
    {
        if (line[i] == '\'')
            parse_quote(line, &i, list);
        else if (line[i] == '\"')
            parse_double_quote(&i, line, list, args);
        else if (line[i] == '$')
            parse_expander_sign(&i, line, list, args);
        else
        {
            add_char_node(list, line[i]);
            i++;
        }
    }
    res = list_to_string(list);
    return (res);
}

char *clean_cmd(char *line, t_args *args)
{
    t_lexems  args_list;
    char    *res;
    char *home_val;

    args_list.head = NULL;
    args_list.tail = NULL;
    if (ft_strchr(line, '~'))
    {
        home_val = get_env("HOME=", args->envp);
        res = ft_strdup(home_val);
        return (res);
    }
    res = clean_line(line, &args_list, args);
    return(res);
}

void lexical_analysis(t_cmd *cmd, t_args *args)
{
    t_execcmd *exec;
    t_redir *rcmd;
    t_pipe *pcmd;
    int i;

    i = 0;
    if (cmd->type == EXEC)
    {
        exec = (t_execcmd *)cmd;
        while (exec->argv[i])
        {
            if (has_parse_symbol(exec->argv[i]))
                exec->argv[i] = clean_cmd(exec->argv[i], args);
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
