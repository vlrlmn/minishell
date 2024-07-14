
#include "./minishell.h"

void	print_content(t_cmd_info *current)
{
	int	i;

	printf("\tINDEX: %d\n", current->index);
	printf("type: %d\n", current->type);
	i = 0;
	if (!current->argv[i])
		printf("NO ARGV!\n");
	while (current->argv[i] != NULL)
	{
		printf("ARGV %s\n", current->argv[i]);
		printf("EARGV %s\n", current->eargv[i]);
		i++;
	}
	printf("redir_type: %d\n", current->redir_type);
	printf("con[0] %d\n", current->connection[0]);
	printf("con[1] %d\n", current->connection[1]);
	printf("fd_read: %d\n", current->fd_read);
	printf("file_read: %s\n", current->file_read);
	printf("fd_write: %d\n", current->fd_write);
	printf("file_write: %s\n", current->file_write);
}

int	PrintList(t_cmd_info *cmd_list)
{
	t_cmd_info	*current;

	current = cmd_list;
	printf("-------------PRINTING LIST-------------\n");
	if (!current)
		return (printf("no list nodes, error\n"), 1);
	while (current->next != NULL)
	{
		print_content(current);
		if (current->next != NULL)
			current = current->next;
		else
			return (printf("ERROR IN PRINTING\n"), 1);
	}
	if (current->next == NULL)
		print_content(current);
	printf("--------------------------------------\n");
	return (0);
}

void	printPipeArr(int **pipe_arr)
{
	int	i;

	i = 0;
	printf("going to print pipe arr\n");
	if (!pipe_arr)
		return ;
	while (pipe_arr[i])
	{
		fprintf(stderr, "pipe_arr[%d][0] = %d\n", i, pipe_arr[i][0]);
		fprintf(stderr, "pipe_arr[%d][1] = %d\n\n", i, pipe_arr[i][1]);
		i++;
	}
}
