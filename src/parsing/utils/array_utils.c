/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 02:52:15 by npapash           #+#    #+#             */
/*   Updated: 2025/04/07 02:52:15 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/*
 * Joins an array of strings with a delimiter
 */
char	*ft_strjoin_array(char **array, char *delimiter)
{
	char	*result;
	char	*temp;
	int		i;

	if (!array || !array[0])
		return (NULL);
	result = ft_strdup(array[0]);
	if (!result)
		return (NULL);
	i = 1;
	while (array[i])
	{
		temp = result;
		result = ft_strjoin(temp, delimiter);
		free(temp);
		if (!result)
			return (NULL);
		temp = result;
		result = ft_strjoin(temp, array[i]);
		free(temp);
		if (!result)
			return (NULL);
		i++;
	}
	return (result);
}
