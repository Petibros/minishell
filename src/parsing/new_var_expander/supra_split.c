/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supra_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 03:10:42 by sacgarci          #+#    #+#             */
/*   Updated: 2025/05/15 13:39:05 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	count_strings(char const *s, char *seps);
int	fill_array(char const *s, char *seps, char **array, int count);
int	is_in_quotes(const char *s, int pos);

static void	ft_free(char **array, int n)
{
	if (n > 0)
	{
		n--;
		while (n >= 0)
		{
			free(array[n]);
			n--;
		}
	}
	free(array);
}

int	is_sep(char c, char *seps)
{
	int	i;

	i = 0;
	while (seps[i])
	{
		if (c == seps[i])
			return (1);
		i++;
	}
	return (0);
}

char	**supra_split(char const *s)
{
	char	*seps;
	char	**array;
	int		count;
	int		i;

	seps = " \t\n\r\f\v";
	if (!s || !seps)
		return (NULL);
	i = 0;
	while (s[i] && is_sep(s[i], seps) && !is_in_quotes(s, i))
		i++;
	count = count_strings(&s[i], seps);
	if (count <= 0)
		return (NULL);
	array = malloc((count + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	count = fill_array(&s[i], seps, array, count);
	if (count >= 0 && !array[count])
	{
		ft_free(array, count);
		return (NULL);
	}
	return (array);
}
