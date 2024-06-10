/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 15:23:28 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/06/10 17:08:50 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void parse_quote(char *line, int *i, t_list *list)
{
    (*i)++;
    while(line[*i] && line[*i] != '\'')
    {
        add_char_node(list, line[*i]);
        (*i)++;
    }
}

void parse_expander_in_quotes(int *i, char *line, t_list *list, t_args *args)
{
    char *env_var;
    char *work_line;
    char *var_name;
    int j;
    int k;

    j = 0;
    k = 0;
    (*i)++;
    work_line = line + *i;
    if (ft_isdigit(work_line[j]))
    {
        k = work_line[j] - '0';
        env_var = ft_itoa(k);
    }
    while(work_line[j] && (ft_isalnum(work_line[j])))
        j++;
    env_var = ft_substr(work_line, 0, j);
    var_name = get_env(env_var, args->envp);
    if (var_name)
        add_str_node(list, var_name);
    (*i) += ft_strlen(var_name);
    free(var_name);
}

void parse_double_quote(int *i, char *line, t_list *list, t_args *args)
{
    char *exit_num;

    (*i)++;
    while(line[*i] && line[*i] != '\"')
    {
        if (line[*i] == '$' && is_delimiter(line[*i + 1]) || line[*i + 1] == '\"')
        {
            add_char_node(list, '$');
            (*i)++;
        }
        else if (line[*i] == '$' && line[*i + 1] =='?')
        {
            exit_num = ft_itoa(line[*i + 1]);
            add_str_node(list, exit_status);
            free(exit_num);
            (*i) += 2;
        }
        else if (line[*i] == '$')
            parse_expander_in_quotes(i, line, list, args);
        else
        {
            add_char_node(list, line[*i]);
            (*i)++;
        }
    }
}

void parse_expander(int *index, t_list *list, char *line, t_args *args)
{
    
}

char *clean_line(char *line, t_list *list, t_args *args)
{
    char *res;
    int i;
    int index;

    i = 0;
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
            add_node(list, line[i]);
            i++;
        }
    }
    res = list_to_string(list);
    return (res);
}

char *clean_cmd (char *line, t_args *args)
{
    t_list  args_list;
    char    *res;
    char *home_val;

    args_list.head = NULL;
    args_list.tail = NULL;
    if (!ft_strchr(line, '~'))
    {
        home_val = get_env("PATH=", args->envp);
        res = ft_strdup(home_val);
        return (res);
    }
    res = clean_line(line, &args_list, args);
    return(res);
}
