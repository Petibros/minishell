/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:58:07 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/15 22:18:57 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <dirent.h>
# include "libft.h"
# include "parser.h"
# include "nodes.h"
# include "utils.h"
# include "expander.h"
# include "wildcard_expander.h"
# include "minishell.h"

/* Token Printing */
void	print_tokens(t_token *head);
void	print_token(t_token *token);

/* AST Printing */
void	print_ast_with_redirs(t_nodes *node, int depth);

#endif
