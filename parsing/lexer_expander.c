/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_expander.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:34:57 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/06/10 17:38:56 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void parse_expander_sign(int *i, char *line, t_lexems *list, t_args *args)
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

void parse_expander(int *i, t_lexems *list, char *line, t_args *args)
{
    int j;
    char *exit_str;

    j = 0;
    (*i)++;
    if(!line[*i] || is_delimiter(line[*i]))
    {
        add_char_node(list, '$');
        return ;
    }
    else if (line[*i] == '?')
    {
        exit_str = ft_itoa(exit_status);
        add_str_node(list, exit_str);
        free(exit_str);
        (*i)++;
    }
    else
        parse_expander_sign(i, line, list, args);
}