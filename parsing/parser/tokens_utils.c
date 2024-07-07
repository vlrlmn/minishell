/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:23:43 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/07 17:24:52 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void skip_delimiters(char **ps, char *es)
{
    while (*ps < es && is_delimiter(**ps))
        (*ps)++;
}

void handle_quotes(char **ps, char *es)
{
    char quote_type = **ps;

    (*ps)++;
    while (*ps < es && **ps != quote_type)
        (*ps)++;
    if (*ps < es)
        (*ps)++;
}
