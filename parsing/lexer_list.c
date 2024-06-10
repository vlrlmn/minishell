/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 15:36:42 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/06/10 17:40:25 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char process_node(t_lexems *list)
{
    char res;
    t_lexem_node *tmp;

    tmp = list->head;
    list->head = list->head->next;
    res = tmp->data;
    free(tmp);
    return(res);
}

char *list_to_string(t_lexems *list)
{
    t_lexem_node *tmp;

    char *str;
    int i;

    i = 0;
    tmp = list->head;
    while(tmp)
    {
        i++;
        tmp = tmp->next;
    }
    str = (char *)malloc(sizeof(char) * (i + 1));
    if (!str)
        return (NULL);
    i = 0;
    while(list->head)
        str[i++] = process_node(list);
    str[i] = '\0';
    return (str);
}

void add_char_node(t_lexems *list, char c)
{
    t_lexem_node *new;

    new = (t_lexem_node *)malloc(sizeof(t_lexem_node));
    if (!new)
        return ;
    new->data = c;
    new->next = NULL;
    if (list->head == NULL)
    {
        list->head = new;
        list->tail = new;
    }
    else
    {
        list->tail->next = new;
        list->tail = new;
    }
}

void add_str_node(t_lexems *list, char *str)
{
    int i;

    i = 0;
    while(str[i])
    {
        add_node(list, str[i]);
        i++;
    }
}
