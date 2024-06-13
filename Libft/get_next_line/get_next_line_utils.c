/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 20:30:06 by sabdulki          #+#    #+#             */
/*   Updated: 2023/08/24 19:03:06 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// char	*ft_strchr(char *c, int ch)
// {
// 	int		i;
// 	char	*arr;

// 	i = 0;
// 	arr = (char *)c;
// 	while (arr[i] != '\0')
// 	{
// 		if (arr[i] == (char)ch)
// 			return (&arr[i]);
// 		i++;
// 	}
// 	if (arr[i] == (char)ch)
// 		return (&arr[i]);
// 	return (0);
// }

int	slen(char *str, int truth)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (0);
	while (str[i] != '\0')
	{
		if (truth == 1 && str[i] == '\n')
			return (i);
		i++;
	}
	return (i);
}

char	*ft_strjoin_gnl(char *s1, char *s2)
{
	int		i_len;
	int		i_main;
	char	*main_arr;

	if (!s1 || !s2)
		return (NULL);
	i_len = 0;
	i_main = 0;
	main_arr = malloc(sizeof(char) * (slen(s1, 0) + slen(s2, 0) + 1));
	if (!main_arr)
		return (NULL);
	while (i_len < slen(s1, 0))
		main_arr[i_main++] = s1[i_len++];
	i_len = 0;
	while (i_len < slen(s2, 0))
		main_arr[i_main++] = s2[i_len++];
	main_arr[i_main] = '\0';
	return (main_arr);
}

void	*free_linked_list(t_list *part)
{
	if (!part)
		return (NULL);
	if (part->next)
		free_linked_list(part->next);
	if (part->content)
		free(part->content);
	free(part);
	return (NULL);
}

char	*create_line(t_list *node, t_list *rem)
{
	char	*line;
	char	*l;

	line = NULL;
	if (node == rem)
		rem = NULL;
	if (!node && rem && !ft_strchr(rem->content, '\n'))
		line = fill_line(rem);
	else if (node)
	{
		line = fill_line(node);
		if (rem)
		{
			l = ft_strjoin_gnl(rem->content, line);
			free(line);
			line = l;
		}
	}
	free_linked_list(rem);
	return (line);
}
