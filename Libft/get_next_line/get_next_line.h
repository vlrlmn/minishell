/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 17:20:12 by sabdulki          #+#    #+#             */
/*   Updated: 2023/08/24 18:49:40 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>

typedef struct s_list
{
	char			*content;
	struct s_list	*next;
}		t_list;

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

t_list	*make_a_node(int fd);
t_list	*create_rem(t_list *last_node);
void	*free_linked_list(t_list *part);
char	*ft_strchr(char *c, int ch);
char	*create_line(t_list *node, t_list *rem);
char	*ft_strjoin_gnl(char *s1, char *s2);
char	*get_next_line(int fd);
char	*fill_line(t_list *node);
int		count_chars_in_line(t_list *node);
int		slen(char *str, int truth);

#endif 