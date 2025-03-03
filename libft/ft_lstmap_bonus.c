/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 19:44:10 by sacgarci          #+#    #+#             */
/*   Updated: 2024/10/13 05:19:39 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*newlist;
	t_list	*node_toadd;

	if (!lst || !f || !del)
		return (NULL);
	newlist = ft_lstnew(f(lst->content));
	lst = lst->next;
	while (lst && newlist)
	{
		node_toadd = ft_lstnew(f(lst->content));
		ft_lstadd_back(&newlist, node_toadd);
		lst = lst->next;
	}
	return (newlist);
}
