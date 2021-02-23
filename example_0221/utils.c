#include "minishell.h"

void    free_two_dim_ary(char **dst)
{
    int     i;

    i = 0;
    while (dst[i])
    {
        free(dst[i]);
        dst[i] = NULL;
        i++;
    }
    free(dst);
    dst = NULL;
}

int     ft_strcmp(const char *s1, const char *s2)
{
    size_t  i;

    i = 0;
    while (s1[i] && (s1[i] == s2[i]))
        i++;
    return (s1[i] - s2[i]);
}