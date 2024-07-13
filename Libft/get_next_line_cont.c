/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_cont.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 20:12:25 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/13 20:13:01 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*join_read(char *save_line, char *buf)
{
	char	*concat;

	concat = ft_strjoin(save_line, buf);
	free(save_line);
	return (concat);
}

char	*remainder_chars(char *save_line)
{
	int		i;
	int		j;
	char	*remainder;

	i = 0;
	if (!save_line)
		return (NULL);
	while (save_line[i] && save_line[i] != '\n')
		i++;
	if (save_line[i] == '\0')
	{
		free(save_line);
		return (NULL);
	}
	remainder = ft_calloc(sizeof(char), (ft_strlen(save_line) - i + 1));
	i++;
	j = 0;
	while (save_line[i])
	{
		remainder[j] = save_line[i];
		j++;
		i++;
	}
	free(save_line);
	return (remainder);
}

char	*final_line(char *save_line)
{
	char	*result;
	int		i;

	i = 0;
	if (!save_line || !save_line[i])
		return (NULL);
	while (save_line[i] && save_line[i] != '\n')
		i++;
	result = ft_calloc(sizeof(char), (i + 1 + 1));
	i = 0;
	while (save_line[i] && save_line[i] != '\n')
	{
		result[i] = save_line[i];
		i++;
	}
	if (save_line[i] && save_line[i] == '\n')
		result[i++] = '\n';
	return (result);
}
