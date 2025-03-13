#include "minishell.h"

void	here_doc(int fd, char *limiter)
{
	char	*buffer;

	buffer = readline("heredoc > ");
	while (ft_strncmp(limiter, buffer, ft_strlen(limiter) + 1))
	{
		write(fd, buffer, ft_strlen(buffer));
		write(fd, "\n", 1);
		free(buffer);
		buffer = readline("heredoc > ");
	}
	free(buffer);
}
