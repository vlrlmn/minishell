/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlomakin <vlomakin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 16:37:50 by vlomakin          #+#    #+#             */
/*   Updated: 2023/07/27 13:01:06 by vlomakin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	if (!*needle)
		return ((char *)haystack);
	if (!len)
		return ((char *) NULL);
	i = 0;
	while (haystack[i] && i < len)
	{
		j = 0;
		while (needle[j] && haystack[i + j] && needle[j] == haystack[i + j] && i
			+ j < len)
			j++;
		if (!needle[j])
			return ((char *)&haystack[i]);
		if (!haystack[i + j] || i + j == len)
			return ((char *) NULL);
		i++;
	}
	return ((char *) NULL);
}
