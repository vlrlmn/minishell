/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlomakin <vlomakin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:23:43 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/14 16:36:46 by vlomakin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	redir_tok_heredoc(char **ps, char **q, char **eq, int *token)
{
	(*ps)++;
	while (is_delimiter(**ps))
		(*ps)++;
	*q = *ps;
	while (!is_delimiter(**ps) && !ft_strchr("|<>", **ps))
		(*ps)++;
	*eq = *ps;
	*token = '+';
	return (*token);
}

int	redir_tok_append(char **ps, char **q, char **eq, int *token)
{
	(*ps)++;
	while (is_delimiter(**ps))
		(*ps)++;
	*q = *ps;
	while (!is_delimiter(**ps) && !ft_strchr("|<>", **ps))
		(*ps)++;
	*eq = *ps;
	*token = '-';
	return (*token);
}
