/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 19:20:08 by npapash           #+#    #+#             */
/*   Updated: 2025/03/23 19:20:08 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "types.h"

/* Utility functions */
void		free_array(char **array);
void		free_token(t_token *token);
void		free_node(t_nodes *node);
char		*ft_strjoin_free(char *s1, char *s2);
void		print_syntax_error(char *token);

/* Debug functions */
void		print_token_list(t_token *token);
void		print_ast(t_nodes *root);

/* Quote handling functions */
char		*remove_quotes(char *str);
int			check_quotes(char *str);
void		handle_quotes_in_node(t_nodes *node);

/* Redirection functions */
int			handle_redirections(t_nodes *node, t_token **token);
void		free_redir_list(t_redir *head);

#endif
