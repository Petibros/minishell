#include "minishell.h"

void	echo(char *msg, int fd, bool option)
{
	if (write(fd, msg, ft_strlen(msg)) == -1)
	{
		perror("echo");
		return ;
	}
	if (!option)
		write(fd, "\n", 1);
}
