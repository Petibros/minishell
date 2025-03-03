/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 01:20:23 by sacgarci          #+#    #+#             */
/*   Updated: 2024/10/11 17:07:27 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	put_in_array(char *nb, int size, long long n)
{
	if (n < 0)
	{
		nb[0] = '-';
		put_in_array(nb, size, -n);
	}
	else if (n >= 10)
	{
		put_in_array(nb, size - 1, n / 10);
		put_in_array(nb, size, n % 10);
	}
	else
	{
		nb[size - 1] = n + 48;
	}
}

char	*ft_itoa(int n)
{
	char		*nb;
	long long	value;
	int			i;

	i = 1;
	value = n;
	if (value < 0)
	{
		value *= -1;
		i++;
	}
	while (value >= 10)
	{
		value /= 10;
		i++;
	}
	nb = malloc((i + 1) * sizeof(char));
	if (!nb)
		return (NULL);
	put_in_array(nb, i, n);
	nb[i] = '\0';
	return (nb);
}
