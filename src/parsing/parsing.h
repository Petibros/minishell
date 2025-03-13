#ifndef PARSING_H
#define PARSING_H

typedef struct s_syntax
{
    int is_command;
    int is_in_redirect;
    int is_out_redirect;
    int is_append_out;
    int is_here_doc;
} t_syntax;

typedef struct s_cmds
{
    t_list  *cmds;
    t_list  *argvs;
    char    **envp;
    char	*in_files;
    char	*out_files;
	int		*append_fd_out;
	char	**delimiter;
	int		*here_doc;
	int		cmd_count;
} t_cmds;

int		parse_line(char *line, char **envp, t_cmds *cmds);
t_list	*tokenize_line(char *line);
int		is_whitespace(char c);
int		skip_whitespace(char *line, int *index);
int     is_syntax_error(t_list *tokens);

#endif