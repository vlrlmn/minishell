#include "minishell.h"

int valid_quotes(char *input)
{
    int	count_single;
	int	count_double;
	int	i;
	int	total_quotes;

	i = 0;
	count_double = 0;
	count_single = 0;
	while (input[i])
	{
		if (input[i] == 34)
			count_double++;
		if (input[i] == 39)
			count_single++;
		i++;
	}
    if(count_single % 2 != 0 || count_double % 2 != 0)
        return(0);
    else 
        return(1);
}

int valid_input(char *work_line)
{
    int i;

    i = 0;
    while(is_delimiter(work_line[i]))
        i++;
    if(!work_line[i])
        return(0);
    if (!valid_quotes(work_line))
    {
        write(2, "Close quotes\n", 19);
        return(0);
    }
}
