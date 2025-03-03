/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 19:38:00 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/03 21:50:19 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new_lst)
{
	t_list	*addlast;

	if (!new_lst || !lst)
		return ;
	addlast = *lst;
	if (*lst)
	{
		while (addlast->next)
		{
			addlast = addlast->next;
		}
		addlast->next = new_lst;
	}
	else
		*lst = new_lst;
}
