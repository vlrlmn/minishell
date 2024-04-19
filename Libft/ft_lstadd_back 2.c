/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlomakin <vlomakin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 10:55:23 by vlomakin          #+#    #+#             */
/*   Updated: 2023/07/27 12:53:03 by vlomakin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*new_ls;

	new_ls = *lst;
	if (!new_ls)
	{
		*lst = new;
		return ;
	}
	new_ls = ft_lstlast(*lst);
	new_ls->next = new;
}
