/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlomakin <vlomakin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 12:33:25 by vlomakin          #+#    #+#             */
/*   Updated: 2023/07/27 13:01:13 by vlomakin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	int		str_begin;
	int		str_end;
	char	*new_str;

	if (!s1 || !set)
		return (NULL);
	str_begin = 0;
	str_end = ft_strlen(s1) - 1;
	while (ft_strchr(set, s1[str_begin]) && str_begin <= str_end)
		str_begin++;
	if (str_begin > str_end)
		return (ft_strdup(""));
	while (ft_strchr(set, s1[str_end]) && str_end >= 0)
		str_end--;
	new_str = (char *)malloc(sizeof(char) * (str_end - str_begin + 2));
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, &s1[str_begin], str_end - str_begin + 2);
	return (new_str);
}
