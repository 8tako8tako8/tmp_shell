#include "pwd.h"

void    ft_pwd(void)
{
    char    path[100];
    int     i;

    i = 0;
    while (i < 100)
    {
        path[i] = 0;
        i++;
    }
    if (!(getcwd(path, 100)))
        exit(1);
    printf("%s\n", path);
}