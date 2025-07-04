/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 04:09:05 by npapash           #+#    #+#             */
/*   Updated: 2025/05/16 13:45:09 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_UTILS_H
# define REDIRECTION_UTILS_H

# include "parsing.h"

// redirection_utils1.c

t_redir	*create_redir_node(char *filename, int append);
void	free_redir_list(t_redir *head);

// redirection_utils2.c
int		handle_input_redirection(t_nodes *node, char *filename);
int		handle_output_redirection(t_nodes *node, char *filename, int append);
int		handle_heredoc(t_nodes *node, char *delimiter);

// redirection_utils3.c
int		process_redirection(t_nodes *node, char *expanded_filename,
			t_token_type type);
int		validate_and_get_token(t_token **token, char **filename);
int		handle_redirection_type(t_nodes *node, t_token **token,
			t_token_type type);
char	*process_quotes_wrapper(char *filename);

// redirection_utils4.c
char	*expand_filename(char *filename, int exit_status,
			char **envp, t_token_type type);
int		handle_redirections(t_nodes *node, t_token **token);

#endif
