#include "parsing.h"
#include "expander.h"

char	*handle_single_quote(char *str, int *i, char *result)
{
	char	*tmp;
	int		start;

	(*i)++;
	start = *i;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	if (*i > start)
	{
		tmp = ft_substr(str, start, *i - start);
		if (!tmp)
			return (NULL);
		result = ft_strjoin_free(result, tmp);
		free(tmp);
	}
	if (str[*i] == '\'')
		(*i)++;
	return (result);
}

int	copy_expanded_entries(char **new_argv, int j, char *arg)
{
	int	count;
	int	old_j;

	old_j = j;
	count = expand_wildcard_entry(arg, new_argv, &j);
	if (count > 0)
		return (j);
	return (old_j + 1);
}
