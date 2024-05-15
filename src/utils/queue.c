/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aajaanan <aajaanan@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:02:46 by aajaanan          #+#    #+#             */
/*   Updated: 2023/09/01 20:01:09 by aajaanan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_queue(t_queue *q)
{
	q->front = NULL;
	q->rear = NULL;
}

void	enqueue(t_queue *q, void *val)
{
	t_queue_node	*new_node;

	new_node = (t_queue_node *)malloc(sizeof(t_queue_node));
	if (!new_node)
		return ;
	new_node->val = val;
	new_node->next = NULL;
	if (!q->front)
	{
		q->front = new_node;
		q->rear = new_node;
	}
	else
	{
		q->rear->next = new_node;
		q->rear = new_node;
	}
}

void	*dequeue(t_queue *q)
{
	t_queue_node	*tmp;
	void			*val;

	if (!q->front)
		return (NULL);
	tmp = q->front;
	val = tmp->val;
	q->front = q->front->next;
	free(tmp);
	return (val);
}

char	*queue_to_str(t_queue *q)
{
	int				len;
	char			*str;
	char			*line;
	t_queue_node	*tmp;

	len = 0;
	tmp = q->front;
	while (tmp)
	{
		len += ft_strlen(tmp->val);
		tmp = tmp->next;
	}
	str = (char *)ft_calloc(len + 1, sizeof(char));
	while (q->front)
	{
		line = dequeue(q);
		ft_strlcat(str, line, len + 1);
		free(line);
	}
	return (str);
}

void	free_queue(t_queue *q)
{
	t_queue_node	*tmp;

	while (q->front)
	{
		tmp = q->front;
		q->front = q->front->next;
		free1(tmp->val);
		free1(tmp);
	}
}
