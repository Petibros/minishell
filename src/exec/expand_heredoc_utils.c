/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <sacgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 15:18:26 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/28 15:21:10 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_digits(char *str, int nbr)
{
	int	n;

	if (!nbr)
		str[0] = 48;
	n = count_digits(nbr) - 1;
	while (nbr)
	{
		str[n] = nbr % 10 + 48;
		nbr /= 10;
		--n;
	}
}

char	*get_expand_name(char *str, int *i)
{
	char	*var;
	int		n;

	n = 0;
	while (str[*i + n] && (ft_isalnum(str[*i + n]) || str[*i + n] == '_'))
		++n;
	var = malloc((n + 1) * sizeof(char));
	if (!var)
		return (NULL);
	ft_strlcpy(var, &str[*i], n + 1);
	*i += n;
	return (var);
}
