/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 15:23:28 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/06/09 15:39:18 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void parse_quote()
{

}

void parse_dollar()
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
        else if (line[i] == '$')
            parse_dollar(&i, list, line, args);
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
