#include "minishell.h"
#include <dirent.h>
#include <stdio.h>

static bool has_wildcard(const char *str)
{
    while (str && *str)
    {
        if (*str == '*' || *str == '?')
            return (true);
        str++;
    }
    return (false);
}

static bool match_pattern(const char *pattern, const char *name)
{
    if (!*pattern && !*name)
        return (true);
    if (*pattern == '*')
        return (match_pattern(pattern + 1, name) 
            || (*name && match_pattern(pattern, name + 1)));
    if (*pattern == '?' && *name)
        return (match_pattern(pattern + 1, name + 1));
    if (*pattern == *name)
        return (match_pattern(pattern + 1, name + 1));
    return (false);
}

static char **find_matches(const char *pattern)
{
    DIR *dir;
    struct dirent *entry;
    char **matches;
    int size;

    dir = opendir(".");
    if (!dir)
    {
        perror("opendir");
        return (NULL);
    }
    matches = NULL;
    size = 0;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.' && pattern[0] != '.')
            continue;
        if (match_pattern(pattern, entry->d_name))
        {
            char **new_matches = malloc(sizeof(char *) * (size + 2));
            if (!new_matches)
            {
                closedir(dir);
                return (matches);
            }
            for (int i = 0; i < size; i++)
                new_matches[i] = matches[i];
            new_matches[size++] = ft_strdup(entry->d_name);
            new_matches[size] = NULL;
            free(matches);
            matches = new_matches;
        }
    }
    closedir(dir);
    return (matches);
}

static int count_args(char **argv)
{
    int count;

    count = 0;
    while (argv && argv[count])
        count++;
    return (count);
}

static char **append_matches(char **argv, int index, char **matches)
{
    int argc;
    int matchc;
    int new_argc;
    char **new_argv;
    int i;
    int j;

    argc = count_args(argv);
    matchc = count_args(matches);
    new_argc = argc - 1 + matchc;
    new_argv = malloc(sizeof(char *) * (new_argc + 1));
    if (!new_argv)
        return (NULL);
    i = 0;
    j = 0;
    while (i < index)
        new_argv[j++] = ft_strdup(argv[i++]);
    for (int k = 0; matches[k]; k++)
        new_argv[j++] = ft_strdup(matches[k]);
    i++;
    while (i < argc)
        new_argv[j++] = ft_strdup(argv[i++]);
    new_argv[j] = NULL;
    return (new_argv);
}

static void free_split(char **split)
{
    int i;

    i = 0;
    while (split && split[i])
        free(split[i++]);
    free(split);
}

void expand_wildcards_in_node(t_nodes *node)
{
    if (!node || !node->argv)
        return;

    for (int i = 0; node->argv[i]; i++)
    {
        if (has_wildcard(node->argv[i]))
        {
            char **matches = find_matches(node->argv[i]);
            if (!matches)
                continue;
            char **new_argv = append_matches(node->argv, i, matches);
            if (!new_argv)
            {
                free_split(matches);
                continue;
            }
            free_split(matches);
            free_split(node->argv);
            node->argv = new_argv;
            i = -1;
        }
    }
}
