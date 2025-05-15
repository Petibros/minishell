#include "minishell.h"

char	**new_copy_array(char **src, char **dest, int *index)
{
	int	i;

	i = 0;
	while (src && src[i])
	{
		dest[*index] = ft_strdup(src[i]);
		(*index)++;
		i++;
	}
	return (dest);
}

char	**new_join_string_arrays(char **arr1, char **arr2)
{
	char	**result;
	int		total_size;
	int		j;

	total_size = new_count_array_size(arr1) + new_count_array_size(arr2);
	result = malloc(sizeof(char *) * (total_size + 1));
	if (!result)
		return (NULL);
	j = 0;
	result = new_copy_array(arr1, result, &j);
	result = new_copy_array(arr2, result, &j);
	result[j] = NULL;
	return (result);
}
